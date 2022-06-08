#include "shell.h"
#define SIZE 256

int main()
{
    // initialize variables and alloc mem
    ret = 0;
    time_str = (char *)calloc(sizeof(char), 8);
    line = (char *)calloc(sizeof(char), SIZE);
    pwd = (char *)calloc(sizeof(char), MAXPATHLEN);
    cmds = (char **)calloc(sizeof(char *), SIZE);
    args = (char **)calloc(sizeof(char *), SIZE);

    pwd = getwd(NULL);

    while (ret < 1) { ret = prompt(); }

    // free memory for graceful exit
    free(pwd);
    free(line);
    free(cmds);
    free(args);
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

        first_cmd = (i == 0);
        last_cmd = (i == cmd_count - 1);
        execute(args, arg_count, first_cmd, last_cmd);
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

int execute(char **args, int arg_count, int first_cmd, int last_cmd)
{
    // check for built-in
    for (int i = 0; i < builtin_count; ++i) {
        if (strcmp(args[0], builtin_names[i]) == 0) {
            if (builtins[i](args) < 0) {
                perror(args[0]);
                return -1;
            }

            return 0;
        }
    }

    // pipe handling, TODO: handle builtins
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

            if (execvp(args[0], args) < 0) { perror("pzash"); }
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
