#pragma once
#ifndef SHELL_H
#define SHELL_H
#include <stdlib.h>
#include <time.h>
#define builtin_count 4

// heap variables
char *pwd;
char *cmd;
char **args;
char **args_pipe;
char *time_str;

// stack variables
int ret;
pid_t pid;
time_t rawtime;
struct tm *timeinfo;

// shell.c functions
void update_time();
int update_pwd();
char **tokenize(char *input, char *delim);
int execute();
int loop();

// built-ins
int cd(char **args);
int help(char **args);
int info(char **args);
int math(char **args);

int (*builtins[builtin_count]) (char **);
char *builtin_names[builtin_count];

#endif
