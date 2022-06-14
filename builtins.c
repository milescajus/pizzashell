#include "shell.h"

time_t rawtime;

// LIST OF BUILT-INS
int (*builtins[]) (char **) = {&cd, &help, &info, &math, &echo, &canal, &hanal};
char *builtin_names[] = {"cd", "help", "info", "math", "echo", "canal", "hanal"};

// HELP TOPICS
enum topics {HELP, TOPICS, BUILTINS, ECHO, PIZZA, CANAL, HANAL};
char *help_topics[] = {"help", "topics", "builtins", "echo", "pizza", "canal", "hanal"};
int topic_count = 7;

// ACTUAL BUILT-IN FUNCTIONS
int help(char **args)
{
    char *topic_str = args[1];
    int topic = -1;

    if (topic_str == NULL) {
        printf("Welcome to PIZZAshell :D\nTry using help with a tasty argument...\n");
        return 0;
    }

    for (int i = 0; i < topic_count; ++i) {
        if (strcmp(topic_str, help_topics[i]) == 0) {
            topic = i;
            break;
        }
    }

    switch (topic) {
        case HELP:
            printf("Usage: help [topic]\n");
            break;
        case TOPICS:
            printf("Help topics: ");
            for (int i = 0; i < topic_count; ++i)
                printf("%s ", help_topics[i]);
            printf("\n");
            break;
        case BUILTINS:
            printf("Available built-in functions:\ncd, help, info, math, echo\n");
            break;
        case ECHO:
            printf("Usage: echo [-n] [\033[4mstring\033[0m]\n");
            break;
        case PIZZA:
            printf("It is delicious. Need I say more?\n");
            break;
        case HANAL:
            printf("Analyze the current directory and all subdirectories. Returns Halstead vocabulary n1: count of distinct operators n2: count of distinct operands.");
            break;
        case CANAL:
            printf("Analyze the current directory and all subdirectories. Returns lines of code for each file with extensions .c, .h, or .py.\n");
            break;
        default:
            printf("undefined topic\n");
            break;
    }

    return 0;
}

int cd(char **args)
{
    char *dest = args[1];

    if (dest == NULL) {
        printf("Usage: cd \033[4mdirectory\033[0m\n");
        return -1;
    }

    // change to previous dir
    if (strcmp(dest, "-") == 0)
        dest = getenv("OLDPWD");

    if (chdir(dest) < 0)
        return -1;

    return update_pwd();
}

int info(char **args)
{
    char *fname = args[1];

    if (fname == NULL || args[2] != NULL) {
        printf("Usage: info \033[4mfilename\033[0m\n");
        return 0;
    }

    struct stat info;
    if (lstat(fname, &info) < 0) { return -1; }

    struct passwd *u_info;
    u_info = getpwuid(info.st_uid);

    char *owner = u_info->pw_name;
    char *filetype = "unknown";
    /*
    char *perms = "00000";
    // itoa(info.st_mode, perms, 8);

    sprintf(perms, "%o", info.st_mode);
    */

    if (S_ISREG(info.st_mode)) { filetype = "regular file"; }
    if (S_ISDIR(info.st_mode)) { filetype = "directory"; }
    if (S_ISCHR(info.st_mode)) { filetype = "character device"; }
    if (S_ISBLK(info.st_mode)) { filetype = "block device"; }
    if (S_ISFIFO(info.st_mode)) { filetype = "named pipe"; }
    if (S_ISLNK(info.st_mode)) { filetype = "symbolic link"; }

    #ifdef __APPLE__
        printf("Name: %s\nSize: %lld B\nOwner: %s\nType: %s\n",
                fname, info.st_size, owner, filetype);
    #else
        printf("Name: %s\nSize: %lu B\nOwner: %s\nType: %s\n",
                fname, info.st_size, owner, filetype);
    #endif
    return 0;
}

int math(char **args)
{
    if (args[1] == NULL || args[2] == NULL || args[3] == NULL) {
        printf("Usage: math \033[4mterm1\033[0m \033[4moperator\033[0m \033[4mterm2\033[0m\n");
        return -1;
    }

    int t1 = (int)strtol(args[1], NULL, 0);
    char op = args[2][0];
    int t2 = (int)strtol(args[3], NULL, 0);

    float res;
    switch (op) {
        case '+':
            res = t1 + t2;
            break;
        case '-':
            res = t1 - t2;
            break;
        case '*':
            res = t1 * t2;
            break;
        case '/':
            res = (float)t1 / (float)t2;
            break;
        default:
            res = 0.f;
            break;
    }

    if (fmod(res, 1.f) == 0)
        printf("%d\n", (int)res);
    else
        printf("%.4f\n", res);

    return 0;
}

int echo(char **args)
{
    int newline = (strcmp(args[1], "-n") == 0) ? 0 : 1;

    int i = newline ? 1 : 2;
    while (args[i]) { printf("%s ", args[i++]); }

    if (newline)
        printf("\n");

    return 0;
}

int canal()
{
    if (!fork()) {
        execlp("python3", "python3", "./complexity/Main.py", NULL);
    } else {
        wait(NULL);
    }

    return 0;
}

int hanal()
{
    if (!fork()) {
        execlp("python3", "python3", "./complexity/Main2.py", NULL);
    } else {
        wait(NULL);
    }

    return 0;
}

// HELPER FUNCTIONS
int update_pwd()
{
    // set OLDPWD to previous dir
    if (setenv("OLDPWD", pwd, 1) < 0)
        return -1;

    // get current dir
    getcwd(pwd, MAXPATHLEN);

    // update PWD to current dir
    if (setenv("PWD", pwd, 1) < 0)
        return -1;

    return 0;
}

void update_time()
{
    rawtime = time(NULL);
    strftime(time_str, 9, "%T", localtime(&rawtime));
}
