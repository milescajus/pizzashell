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
        switch (pid) {
            case -1: // FAILED
                fprintf(stderr, "FORK FAILED");
                return -1;
            case 0: // CHILD PROC
                execvp(line, args);
                break;
            default: // PARENT
                wait(NULL);
                break;
        }

    }

    return -1;
}
