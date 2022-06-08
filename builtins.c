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
int help()
{
    printf("Welcome to PIZZAshell :D\n");
    return 0;
}

int cd()
{
    // change to previous dir
    if (strcmp(args[1], "-") == 0)
        args[1] = getenv("OLDPWD");

    // change to home dir, TODO: tilde-expansion
    if (strcmp(args[1], "~") == 0)
        args[1] = getenv("HOME");

    if (chdir(args[1]) < 0)
        return -1;

    return update_pwd();
}

int info()
{
    struct stat info;
    stat(args[1], &info);
    printf("%lu, %u, %u\n", info.st_size, info.st_uid, info.st_mode);
    return 0;
}

int math()
{
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
