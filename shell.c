#include "shell.h"
#define SIZE 256

int main()
{
    // initialize variables and alloc mem
    ret = 0;
    time_str = (char *)calloc(sizeof(char), 8);
    line = (char *)calloc(sizeof(char), SIZE);
    pwd = (char *)calloc(sizeof(char), MAXPATHLEN);
    cmds = (char **)calloc(sizeof(char *), SIZE);
    args = (char **)calloc(sizeof(char *), SIZE);

    pwd = getwd(NULL);

    while (ret < 1) { ret = prompt(); }

    // free memory for graceful exit
    free(pwd);
    free(line);
    free(cmds);
    free(args);
    free(time_str);

    printf("\n");
    exit(0);
}

int prompt()
{
    // main prompt loop

    update_time();

    // print prompt
    printf("\n\033[31;1m%s\033[0m [%s]\n", pwd, time_str);
    line = readline("🍕\033[38;5;220m$\033[0m ");

    // handle Ctrl-D
    if (line == NULL)
        return 1;

    // only execute actual commands
    if (strlen(line) == 0)
        return 0;

    // add line to readline history
    add_history(line);

    int cmd_count = tokenize(cmds, line, "|");

    for (int i = 0; i < cmd_count; ++i) {
        if (cmds[i][0] == ' ')  // command after | pipe has leading space
            cmds[i]++;

        int arg_count = tokenize(args, cmds[i], " ");

        execute(args, arg_count);
    }

    return 0;
}

int tokenize(char **dest, char *source, char *delim)
{
    // splits a char array by spaces and adds terminating null

    int len = 0;
    while ((dest[len] = strsep(&source, delim)) != NULL) { len++; }

    // rudimentary way to terminate at 2 or more spaces
    for (int i = 0; i < len; ++i) {
        if (strcmp(dest[i], "") == 0) {
            dest[i] = NULL;
            break;
        }
    }

    return len;
}

int pipe_execute()
{
    /*
    // execute i to i+1 then pass back output as input for i+2 etc...
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i <= SIZE; i++)
    {
        if(!fork())
        {
            //process input to pipe
            write(STDIN_FILENO, args[i], SIZE);
            dup2(fd[WRITE_END], STDOUT_FILENO);
            close(fd[WRITE_END]);
            loop();
        }

        else
        {
            dup2(fd[READ_END], STDIN_FILENO);
            close(fd[READ_END]);
            wait(NULL);
            loop();
        }
    }
    */
        return 0;

    /*

    // first iterations makes the ouput into the input for next


    int fd[2];
    write(STDIN_FILENO, STDOUT_FILENO);
    if(!fork())
    {
    //process input to pipe
    dup2(fd[WRITE_END], STDOUT_FILENO);
    close(fd[WRITE_END]);
    loop();
    ;
    }

    else
    {
    wait(NULL);
    dup2(fd[READ_END], STDIN_FILENO);
    close(fd[READ_END]);
    loop();
    }
    }
    }
    */
        }

int execute(char **args, int arg_count)
{
    // check for built-in
    for (int i = 0; i < builtin_count; ++i) {
        if (strcmp(args[0], builtin_names[i]) == 0) {
            if (builtins[i](args) < 0) {
                perror(args[0]);
                return -1;
            }

            return 0;
        }
    }

    // must be external program, need to fork
    pid = fork();

    switch (pid) {
        case 0:
            // CHILD
            if (execvp(args[0], args) < 0) { perror("pzash"); }
            exit(-1);    // only runs if execvp fails anyway

        case -1:
            // ERROR
            fprintf(stderr, "fork failed\n");
            break;

        default:
            // PARENT
            wait(NULL);
            break;
    }

    return 0;
}
