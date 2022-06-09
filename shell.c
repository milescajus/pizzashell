#include "shell.h"
#define SIZE 256

// heap variables
char *pwd;      // current directory
char *line;     // user input, malloc by readline
char **cmds;    // array of commands
char **args;    // array of args per command
char *time_str; // to hold current time

// stack variables
int fd1[2];     // first pipe filedesc
int fd2[2];     // second pipe filedesc
pid_t pid;
time_t rawtime;
struct tm *timeinfo;

int main(int argc, char *argv[])
{
    int ret = 0;

    // initialize heap variables
    time_str = (char *)calloc(sizeof(char), 8);
    pwd = (char *)calloc(sizeof(char), MAXPATHLEN);
    cmds = (char **)calloc(sizeof(char *), SIZE);
    args = (char **)calloc(sizeof(char *), SIZE);

    getcwd(pwd, MAXPATHLEN);

    if (argc > 1 && strcmp(argv[1], "-c") == 0) {
        // run command directly

        for (int i = 2; i < argc; ++i)
            // copy args from argv
            args[i - 2] = argv[i];

        execute(args, 1, 1);

    } else {
        // start interactive shell

        while (ret < 1) { ret = prompt(); }
        free(line);

    }

    // free memory for graceful exit
    free(args);
    free(cmds);
    free(pwd);
    free(time_str);

    printf("\n");
    exit(0);
}

int prompt()
{
    // main prompt loop

    update_time();

    // print prompt
    printf("\n\033[31;1m%s\033[0m [%s]\n", pwd, time_str);
    line = readline("🍕\033[38;5;220m$\033[0m ");

    // handle Ctrl-D
    if (line == NULL)
        return 1;

    // only execute actual commands
    if (strlen(line) == 0)
        return 0;

    // add line to readline history
    add_history(line);

    int cmd_count = tokenize(cmds, line, "|");
    int first_cmd;
    int last_cmd;

    for (int i = 0; i < cmd_count; ++i) {
        if (cmds[i][0] == ' ')  // command after | pipe has leading space
            cmds[i]++;

        int arg_count = tokenize(args, cmds[i], " ");
        // expand(args, arg_count);

        first_cmd = (i == 0);
        last_cmd = (i == cmd_count - 1);
        execute(args, first_cmd, last_cmd);
    }

    return 0;
}

int tokenize(char **dest, char *source, char *delim)
{
    // splits a char array by spaces and adds terminating null

    int len = 0;
    while ((dest[len] = strsep(&source, delim)) != NULL) { len++; }

    // rudimentary way to terminate at 2 or more spaces
    for (int i = 0; i < len; ++i) {
        if (strcmp(dest[i], "") == 0) {
            dest[i] = NULL;
            break;
        }
    }

    return len;
}

void expand(char **args, int len)
{
    // replaces special chars like ~, $, *

    for (int i = 0; i < len; ++i) {
        printf("arg[%d]: %s", i, args[i]);
        if (args[i][0] == '$') {
            char *envar = strsep(&args[i], "/");    // get everything until first '/' or '\0'
            envar = getenv(++envar);                // replace $XX with env[XX], DON'T MODIFY!

            char *newarg = (char *)malloc(sizeof envar + sizeof args[i] + 1);   // +1 for '/'
            strcpy(newarg, envar);
            strcat(newarg, "/");

            args[i] = (args[i] == NULL) ? newarg : strcat(newarg, args[i]);
            // gets freed in main()
        }
    }
}

int execute(char **args, int first_cmd, int last_cmd)
{
    char *command = args[0];

    // check for built-in
    for (int i = 0; i < builtin_count; ++i) {
        if (strcmp(command, builtin_names[i]) == 0) {
            if (builtins[i](args) < 0) {
                perror(command);
                return -1;
            }

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

            wait(NULL);
            break;
    }

    return 0;
}
