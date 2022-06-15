#include "shell.h"
#define SIZE 256

// heap variables
char *pwd;      // current directory
char *line;     // user input, malloc by readline
char **cmds;    // array of commands
char **args;    // array of args per command
wordexp_t p;

// stack variables
int fd1[2];     // first pipe filedesc
int fd2[2];     // second pipe filedesc
pid_t pid;
char time_str[9];
int status;
int exit_status;

int main(int argc, char *argv[])
{
    int ret = 0;

    // initialize heap variables
    pwd = (char *)calloc(sizeof(char), MAXPATHLEN);
    cmds = (char **)calloc(sizeof(char *), SIZE);
    args = (char **)calloc(sizeof(char *), SIZE);

    getcwd(pwd, MAXPATHLEN);

    if (argc > 1 && strcmp(argv[1], "-c") == 0) {
        // run command directly

        /*
        for (int i = 0; i < argc; ++i)
            // copy args from argv
            *(args + i) = argv[i + 2];
        */
        if (argv[2] == NULL) {
            puts("Usage: pzash -c [\033[4mcommand\033[0m]");
            return -1;
        }

        parse_run(argv[2]);

    } else {
        // start interactive shell

        while (ret < 1) { ret = prompt(); }
        free(line);

    }

    // free memory for graceful exit
    free(args);
    free(cmds);
    free(pwd);
    wordfree(&p);

    printf("\n");
    exit(0);
}

int prompt()
{
    // main prompt loop

    // ensure fresh memory for new input
    for (int i = 0; i < SIZE; ++i) {
        args[i] = NULL;
        cmds[i] = NULL;
    }

    update_time();

    // print prompt
    printf("\n\033[31;1m%s\033[0m [%s]", pwd, time_str);
    if (exit_status)
        printf(" C: \033[91m%d\033[0m", exit_status);
    printf("\n");
    line = readline("🍕\033[38;5;220m$\033[0m ");

    // handle Ctrl-D
    if (line == NULL)
        return 1;

    // handle empty input
    if (*line == '\0')
        return 0;

    // add line to readline history
    add_history(line);

    // parse and execute
    parse_run(line);

    return 0;
}

void parse_run(char *line)
{
    int cmd_count = tokenize(cmds, line, "|");

    int first_cmd;
    int last_cmd;

    for (int i = 0; i < cmd_count; ++i) {
        if (expand(args, *(cmds + i)) < 0)
            return;

        first_cmd = (i == 0);
        last_cmd = (i == cmd_count - 1);

        if (execute(args, first_cmd, last_cmd) < 0)
            return;
    }
}

int tokenize(char **dest, char *source, char *delim)
{
    // splits source by delimiter into destination array
    // skips leading delims and empties

    // skip leading delims
    while (*source == *delim) { source++; }

    int len = 0;
    while ((dest[len] = strsep(&source, delim))) {
        if (*dest[len] != '\0') len++;  // only iterate on non-empty
    }

    return len;
}

int expand(char **dest, char *source)
{
    // splits by whitespace and performs sh-like word expansion
    // i.e. replaces special chars like ~, $, *

    int len = p.we_wordc;

    int err = wordexp(source, &p, 0);
    switch (err) {
        case WRDE_BADCHAR:
            puts("illegal character");
            break;
        case WRDE_BADVAL:
            puts("undefined variable");
            break;
        case WRDE_NOSPACE:
            puts("out of memory");
            break;
        case WRDE_SYNTAX:
            puts("syntax error");
            break;
    }

    if (err)
        return -1;

    for (int i = 0; i < p.we_wordc; ++i) {
        dest[i] = p.we_wordv[i];
    }

    return len;
}

/*
int expand_old(char **args, int len)
{
    // performs sh-like word expansion
    // i.e. replaces special chars like ~, $, *

    for (int i = 1; i < len; ++i) {
        char **w;

        // expand special characters
        int err = wordexp(*(args + i), &p, 0);
        switch (err) {
            case WRDE_BADCHAR:
                puts("illegal character");
                break;
            case WRDE_BADVAL:
                puts("undefined variable");
                break;
            case WRDE_NOSPACE:
                puts("out of memory");
                break;
            case WRDE_SYNTAX:
                puts("syntax error");
                break;
        }

        if (err)
            return -1;

        w = p.we_wordv;

        int offset = p.we_wordc - 1;

        // shift args to account for expansion
        for (int j = len - 1 + offset; j > i; --j) {
            args[j] = args[j - offset];
        }

        // insert new args into empty space
        for (int j = 0; j < p.we_wordc; ++j) {
            args[i + j] = w[j];
        }

        // update total len
        len += offset;
    }

    return 0;
}
*/

int execute(char **args, int first_cmd, int last_cmd)
{
    char *command = *args;

    if (command == NULL) {
        fprintf(stderr, "How did this happen? We're smarter than this.\n");
        exit(-1);
    }

    // check for built-in
    for (int i = 0; i < builtin_count; ++i) {
        if (strcmp(command, builtin_names[i]) == 0) {
            if (builtins[i](args) < 0) {
                perror(command);
                return -1;
            }

            exit_status = 0;
            return 0;
        }
    }

    // pipe handling
    if (!last_cmd) {
        if (pipe(fd2) < 0) {
            perror("PIPE ERROR");
            return -1;
        }
    }

    // must be external program, need to fork
    pid = fork();

    switch (pid) {
        case -1:
            // ERROR
            fprintf(stderr, "fork failed\n");
            break;

        case 0:
            // CHILD
            if (!first_cmd) {
                dup2(fd1[0], 0);
                close(fd1[0]);
                close(fd1[1]);
            }

            if (!last_cmd) {
                close(fd2[0]);
                dup2(fd2[1], 1);
                close(fd2[1]);
            }

            if (execvp(command, args) < 0) { perror("pzash"); }
            exit(-1);    // only runs if execvp fails anyway

        default:
            // PARENT
            if (!first_cmd) {
                close(fd1[0]);
                close(fd1[1]);
            }

            if (!last_cmd) {
                fd1[0] = fd2[0];
                fd1[1] = fd2[1];
            }

            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                exit_status = WEXITSTATUS(status);
            break;
    }

    return 0;
}
