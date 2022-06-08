#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/param.h>
#ifdef __APPLE__
    #include <editline/readline.h>
#else
    #include <readline/readline.h>
    #include <readline/history.h>
#endif
#ifdef __FreeBSD__
    #include <sys/wait.h>
#endif

#include "shell.h"
#define SIZE 256

int main()
{
    // initialize variables and alloc mem
    ret = 0;
    time_str = (char *)calloc(sizeof(char), 8);
    cmd = (char *)calloc(sizeof(char), SIZE);
    pwd = (char *)calloc(sizeof(char), MAXPATHLEN);
    args = (char **)calloc(sizeof(char *), SIZE);

    pwd = getwd(NULL);

    while (ret < 1) { ret = loop(); }

    // free memory for graceful exit
    free(pwd);
    free(cmd);
    free(args);
    free(time_str);

    printf("\n");
    exit(0);
}

int loop()
{
    // main prompt loop

    update_time();

    // print prompt
    printf("\n\033[31;1m%s\033[0m [%s]\n", pwd, time_str);
    cmd = readline("🍕\033[38;5;220m$\033[0m ");

    // handle Ctrl-D
    if (cmd == NULL)
        return 1;

    // only add actual commands to hist
    if (strlen(cmd) > 0) {
        add_history(cmd);
        args = tokenize(cmd, "|");
        // handle piping
        //printf("before pipe %s", args[1]);
        if( args[1] != NULL )
        {
          pipe_execute();
        }
        // endpiping
        args = tokenize(cmd, " ");
        return execute();
    }

    return 0;
}

char **tokenize(char *input, char *delim)
{
    // splits a char array by spaces and adds terminating null

    int len = 0;
    while ((args[len] = strsep(&input, delim)) != NULL) { len++; }

    // rudimentary way to terminate at 2 or more spaces
    for (int i = 0; i < SIZE; ++i) {
        if (args[i] == NULL)
            break;
        if (strcmp(args[i], "") == 0) {
            args[i] = NULL;
            break;
        }
    }

    // TODO: replace $XX with envars
    // TODO: expand ~ to $HOME

    return args;
}
int pipe_execute()
{
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
}

int execute()
{
    // if user enters nothing
    if (cmd[0] == '\0')
        return 0;

    // check for built-in
    for (int i = 0; i < builtin_count; ++i) {
        if (strcmp(cmd, builtin_names[i]) == 0) {
            if (builtins[i](args) < 0) {
                perror(cmd);
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
            if (execvp(cmd, args) < 0) { perror("pzash"); }
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
