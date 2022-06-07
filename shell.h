#pragma once
#ifndef SHELL_H
#define SHELL_H

int update_pwd();
char **tokenize(char *input);
int execute();
int loop();

// built-ins
const int builtin_count;

int cd(char **args);
int help(char **args);

int (*builtins[2]) (char **);
char *builtin_names[2];

#endif
