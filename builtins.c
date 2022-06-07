#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "shell.h"

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
