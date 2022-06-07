#pragma once
#ifndef SHELL_H
#define SHELL_H
#include <stdlib.h>
#include <time.h>

// heap variables
char *pwd;
char *cmd;
char **args;
char *time_str;

// stack variables
int ret;
pid_t pid;
time_t rawtime;
struct tm *timeinfo;

// shell.c functions
void update_time();
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
