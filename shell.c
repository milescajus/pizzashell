#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/param.h>
#include <editline/readline.h>

#include "shell.h"
#define SIZE 256

int main()
{
    // initialize variables and alloc mem
    ret = 0;
    time_str = (char *)calloc(sizeof(char), 8);
    cmd = (char *)calloc(sizeof(char), SIZE);
    pwd = (char *)calloc(sizeof(char), MAXPATHLEN);
    args = (char **)calloc(sizeof(char *), SIZE);

    pwd = getwd(NULL);

    while (ret < 1) { ret = loop(); }

    // free memory for graceful exit
    free(pwd);
    free(cmd);
    free(args);
    free(time_str);

    exit(0);
}

int loop()
{
    // main prompt loop

    update_time();

    // print prompt
    printf("\n\033[31;1m%s\033[0m [%s]\n", pwd, time_str);
    cmd = readline("🍕\033[38;5;220m$\033[0m ");

    // handle Ctrl-D
    if (cmd == NULL)
        return 1;

    // only add actual commands to hist
    if (strlen(cmd) > 0)
        add_history(cmd);

    args = tokenize(cmd);

    return execute();
}

char **tokenize(char *input)
{
    // splits a char array by spaces and adds terminating null

    int len = 0;
    while ((args[len] = strsep(&input, " ")) != NULL) { len++; }

    // rudimentary way to terminate at 2 or more spaces
    for (int i = 0; i < SIZE; ++i) {
        if (args[i] == NULL)
            break;
        if (strcmp(args[i], "") == 0) {
            args[i] = NULL;
            break;
        }
    }

    return args;
}

int execute()
{
    // if user enters nothing
    if (cmd[0] == '\0')
        return 0;

    // check for built-in
    for (int i = 0; i < builtin_count; ++i) {
        if (strcmp(cmd, builtin_names[i]) == 0) {
            if (builtins[i](args) < 0) {
                perror(cmd);
                return -1;
            }

            return 0;
        }
    }

    // must be external program, need to fork
    pid = fork();

    switch (pid) {
        case 0:
            // CHILD
            if (execvp(cmd, args) < 0) { perror("pzash"); }
            exit(-1);    // only runs if execvp fails anyway

        case -1:
            // ERROR
            fprintf(stderr, "fork failed\n");
            break;

        default:
            // PARENT
            wait(NULL);
            break;
    }

    return 0;
}
