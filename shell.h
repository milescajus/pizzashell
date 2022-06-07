#pragma once
#ifndef SHELL_H
#define SHELL_H
#include <stdlib.h>

pid_t pid;
char *pwd;      // dynamically allocated
char *cmd;      // dynamically allocated
char **args;    // dynamically allocated

int update_pwd();
char **tokenize(char *input);
int execute();
int loop();

// built-ins
const int builtin_count;

int cd();
int help();

int (*builtins[2]) (char **);
char *builtin_names[2];

#endif
