#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char **tokenize(char * input);
void execute(char * cmd, char ** args);
int loop();
