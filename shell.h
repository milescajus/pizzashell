#pragma once
#ifndef SHELL_H
#define SHELL_H

#include <limits.h>
#include <math.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wordexp.h>

#ifdef __APPLE__
    #include <editline/readline.h>
#else
    #include <readline/readline.h>
    #include <readline/history.h>
    #include <sys/wait.h>
#endif

#define builtin_count 5

// heap variables
extern char    *pwd;        // current directory
extern char    *line;       // user input, malloc by readline
extern char   **cmds;       // array of commands
extern char   **args;       // array of args per command

// stack variables
extern int      nextfd[2];  // next pipe fildes
extern int      prevfd[2];  // previous pipe fildes
extern pid_t    pid;
extern time_t   rawtime;
extern char     time_str[9];
extern int      exit_status;

// shell.c functions
int     prompt();
void    parse_run(char *line);
int     tokenize(char **dest, char *source, char *delim);
int     expand(char **dest, char *source);
int     execute(char **args, int first_cmd, int last_cmd);

// built-in functions
int     cd(char **args);
int     help(char **args);
int     info(char **args);
int     math(char **args);
int     echo(char **args);

// helper functions
int     update_time();
int     update_pwd();
char   *build_prompt();

// function pointers
extern int (*builtins[builtin_count]) (char **);
extern char *builtin_names[builtin_count];

#endif
