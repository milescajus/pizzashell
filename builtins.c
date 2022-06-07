#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "shell.h"

// LIST OF BUILT-INS
const int builtin_count = 2;
int (*builtins[]) (char **) = {&cd, &help};
char *builtin_names[2] = {"cd", "help"};

// ACTUAL BUILT-IN FUNCTIONS
int help()
{
    printf("Welcome to PIZZAshell :D\n");
    return 0;
}

int cd()
{
    if (strcmp(args[1], "-") == 0) {
        args[1] = getenv("OLDPWD");
    }

    if (chdir(args[1]) < 0)
        return -1;

    return update_pwd();
}

// HELPER FUNCTIONS
int update_pwd()
{
    if (setenv("OLDPWD", pwd, 1) < 0)
        return -1;

    pwd = getwd(NULL);

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
