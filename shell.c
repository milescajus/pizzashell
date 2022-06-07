#include "shell.h"
#define SIZE 256

char **tokenize(char * input)
{
    char **args = (char **)calloc(sizeof(char *), SIZE);
    int i = 0;
    while ((args[i] = strsep(&input, " ")) != NULL) { i++; }
    args[i] = NULL;
    args = realloc(args, sizeof(char *) * i);

    return args;
}

int main()
{
    pid_t pid;

    while (1) {
        char *line = (char *)calloc(sizeof(char), SIZE);
        void *in = fgets(line, SIZE, stdin);

        if (in == NULL)
            return 0;

        line[strlen(line) - 1] = '\0';

        char **args = tokenize(line);

        pid = fork();

        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            break;
        } else if (pid == 0) {
            if (execvp(line, args) < 0) {
                fprintf(stderr, "exec failed\n");
                perror(line);
            }
            break;
        } else {
            wait(NULL);
        }

        free(line);
        free(args);
    }

    return -1;
}
