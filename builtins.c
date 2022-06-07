#include <stdio.h>
#include <unistd.h>

int help(char **args)
{
    printf("Welcome to PIZZAshell :D\n");
    return 0;
}

int cd(char **args)
{
    return chdir(args[1]);
}
