#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>

#include "shell.h"
#define SIZE 256

pid_t pid;
char *pwd;      // dynamically allocated
char *cmd;      // dynamically allocated
char **args;    // dynamically allocated
void *in;

int main()
{
    cmd = (char *)calloc(sizeof(char), SIZE);
    pwd = (char *)calloc(sizeof(char), MAXPATHLEN);
    args = (char **)calloc(sizeof(char *), SIZE);

    while (1) { loop(); }

    free(pwd);
    free(cmd);
    free(args);
    return 0;
}

int loop()
{
    update_env();
    printf("\n%s\n", getenv("PWD"));
    printf("🍕> ");
    in = fgets(cmd, SIZE, stdin);

    if (in == NULL)
        exit(0);

    cmd[strlen(cmd) - 1] = '\0';

    args = tokenize(cmd);

    return execute();
}

int update_env()
{
    getcwd(pwd, MAXPATHLEN);
    return setenv("PWD", pwd, 1);
}

char **tokenize(char *input)
{
    int i = 0;
    while ((args[i] = strsep(&input, " ")) != NULL) { i++; }
    args[i] = NULL;

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

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        return -1;
    } else if (pid == 0) {
        if (execvp(cmd, args) < 0) {
            perror("pzash");
            exit(-1);
        }
    } else {
        wait(NULL);
    }

    return 0;
}
