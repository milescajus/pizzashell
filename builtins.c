#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "shell.h"

const int builtin_count = 2;
int (*builtins[]) (char **) = {&cd, &help};
char *builtin_names[2] = {"cd", "help"};

int help(char **args)
{
    printf("Welcome to PIZZAshell :D\n");
    return 0;
}

int cd(char **args)
{
    if (strcmp(args[1], "-") == 0) {
        args[1] = getenv("OLDPWD");
    }

    setenv("OLDPWD", getenv("PWD"), 1);

    return chdir(args[1]);
}
