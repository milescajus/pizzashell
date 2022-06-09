#pragma once
#ifndef SHELL_H
#define SHELL_H

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#ifdef __APPLE__
    #include <editline/readline.h>
#else
    #include <readline/readline.h>
    #include <readline/history.h>
#endif
#ifdef __FreeBSD__
    #include <sys/wait.h>
#endif

#define builtin_count 6

// heap variables
char *pwd;      // current directory
char *line;     // user input, malloc by readline
char **cmds;    // array of commands
char **args;    // array of args per command
char *time_str;

// stack variables
int ret;
int fd1[2];
int fd2[2];
pid_t pid;
time_t rawtime;
struct tm *timeinfo;

// shell.c functions
void update_time();
int update_pwd();
int prompt();
int tokenize(char **dest, char *source, char *delim);
int expand(char **args, int len);
int execute(char **args, int first_cmd, int last_cmd);

// built-ins
int cd(char **args);
int help(char **args);
int info(char **args);
int math(char **args);
int echo(char **args);
int canal();

int (*builtins[builtin_count]) (char **);
char *builtin_names[builtin_count];

#endif
