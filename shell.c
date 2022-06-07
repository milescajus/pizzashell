#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell.h"
#define SIZE 256

pid_t pid;
char *cmd;      // dynamically allocated
char **args;    // dynamically allocated
void *in;

int main()
{
    int res = loop();

    free(cmd);
    free(args);
    return res;
}

int loop()
{
    while (1) {
        printf("🍕> ");
        cmd = (char *)calloc(sizeof(char), SIZE);
        in = fgets(cmd, SIZE, stdin);

        if (in == NULL)
            break;

        cmd[strlen(cmd) - 1] = '\0';

        args = tokenize(cmd);

        pid = fork();

        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            break;
        } else if (pid == 0) {
            if (execute() < 0)
                return -1;
            break;
        } else {
            wait(NULL);
        }

    }

    return 0;
}

char **tokenize(char *input)
{
    char **args = (char **)calloc(sizeof(char *), SIZE);
    int i = 0;
    while ((args[i] = strsep(&input, " ")) != NULL) { i++; }
    args[i] = NULL;

    return args;
}

int execute()
{
    if (cmd[0] == '\0')
        return 0;

    for (int i = 0; i < builtin_count; ++i) {
        if (strcmp(cmd, builtin_names[i]) == 0)
            return builtins[i](args);
    }

    if (execvp(cmd, args) < 0) {
        fprintf(stderr, "exec failed\n");
        perror(cmd);
        return -1;
    }

    return 0;
}
