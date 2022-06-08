#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "shell.h"

// LIST OF BUILT-INS
int (*builtins[]) (char **) = {&cd, &help, &info, &math};
char *builtin_names[] = {"cd", "help", "info", "math"};

// ACTUAL BUILT-IN FUNCTIONS
int help(char **args)
{
    char *topic = args[1];

    printf("Welcome to PIZZAshell :D\n");
    if (topic != NULL) {
        printf("Selected topic: %s\n", topic);
    }

    return 0;
}

int cd(char **args)
{
    char *dest = args[1];

    // change to previous dir
    if (strcmp(dest, "-") == 0)
        dest = getenv("OLDPWD");

    if (dest[0] == '~') {
        strcpy(dest, getenv("HOME"));
        strcat(dest, ++args[1]);
    }

    if (chdir(dest) < 0)
        return -1;

    return update_pwd();
}

int info(char **args)
{
    char *fname = args[1];

    struct stat info;
    stat(fname, &info);
    printf("%lu, %u, %u\n", info.st_size, info.st_uid, info.st_mode);
    return 0;
}

int math(char **args)
{
    int t1 = (int)strtol(args[1], NULL, 0);
    char op = args[2][0];
    int t2 = (int)strtol(args[3], NULL, 0);

    float res;
    switch (op) {
        case '+':
            res = t1 + t2;
            break;
        case '-':
            res = t1 - t2;
            break;
        case '*':
            res = t1 * t2;
            break;
        case '/':
            res = (float)t1 / (float)t2;
            break;
    }

    if (fmod(res, 1.f) == 0)
        printf("%d\n", (int)res);
    else
        printf("%.4f\n", res);

    return 0;
}

// HELPER FUNCTIONS
int update_pwd()
{
    // set OLDPWD to previous dir
    if (setenv("OLDPWD", pwd, 1) < 0)
        return -1;

    // get current dir
    pwd = getwd(NULL);

    // update PWD to current dir
    if (setenv("PWD", pwd, 1) < 0)
        return -1;

    return 0;
}

void update_time()
{
    rawtime = time(NULL);
    timeinfo = localtime(&rawtime);
    strftime(time_str, 8, "%H:%M:%S", timeinfo);
}
