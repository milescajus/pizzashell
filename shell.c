#include "shell.h"
#define SIZE 256

int main()
{
    pid_t pid;

    while (1) {
        char line[SIZE];
        void *in = fgets(line, SIZE, stdin);

        if (in == NULL)
            return 0;

        char *args[] = {line, NULL};

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

    return -1;
}
