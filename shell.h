#pragma once

int update_env();
char **tokenize(char *input);
int execute();
int loop();

// built-ins
const int builtin_count = 2;

int cd(char **args);
int help(char **args);

int (*builtins[]) (char **) = {&cd, &help};
char *builtin_names[] = {"cd", "help"};
