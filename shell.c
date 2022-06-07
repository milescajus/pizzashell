#include "shell.h"
#define SIZE 256

char **tokenize(char * input)
{
    char **args = (char **)calloc(sizeof(char *), SIZE);
    int i = 0;
    while ((args[i] = strsep(&input, " ")) != NULL) { i++; }
    args[i] = NULL;

    return args;
}

int main()
{
    pid_t pid;
    char *line;
    void *in;
    char **args;

    while (1) {
        printf("🍕> ");
        line = (char *)calloc(sizeof(char), SIZE);
        in = fgets(line, SIZE, stdin);

        if (in == NULL)
            break;

        line[strlen(line) - 1] = '\0';

        args = tokenize(line);

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

    }

    free(line);
    free(args);
    return 0;
}
