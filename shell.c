#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/param.h>
#include <editline/readline.h>

#include "shell.h"
#define SIZE 256

int ret;
time_t rawtime;
struct tm *timeinfo;
char *time_str;

int main()
{
    ret = 0;
    time_str = (char *)calloc(sizeof(char), 8);
    cmd = (char *)calloc(sizeof(char), SIZE);
    pwd = (char *)calloc(sizeof(char), MAXPATHLEN);
    args = (char **)calloc(sizeof(char *), SIZE);

    pwd = getwd(NULL);

    while (ret < 1) { ret = loop(); }

    free(pwd);
    free(cmd);
    free(args);

    exit(0);
}

int loop()
{
    update_time();
    printf("\n\033[31;1m%s\033[0m [%s]\n", pwd, time_str);
    cmd = readline("🍕> ");

    if (cmd == NULL)
        return 1;

    if (strlen(cmd) > 0)
        add_history(cmd);

    args = tokenize(cmd);

    return execute();
}

void update_time()
{
    rawtime = time(NULL);
    timeinfo = localtime(&rawtime);
    strftime(time_str, 8, "%H:%M:%S", timeinfo);
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
