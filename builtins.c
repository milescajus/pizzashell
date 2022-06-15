#include "shell.h"

time_t rawtime;

// LIST OF BUILT-INS
int (*builtins[]) (char **) = {&cd, &help, &info, &math, &echo, &canal, &hanal};
char *builtin_names[] = {"cd", "help", "info", "math", "echo", "canal", "hanal"};

// HELP TOPICS
enum topics {HELP, TOPICS, BUILTINS, CD, INFO, MATH, ECHO, PIZZA, CANAL, HANAL};
char *help_topics[] = {"help", "topics", "builtins", "cd", "info", "math", "echo", "pizza", "canal", "hanal"};
int topic_count = 10;

// ACTUAL BUILT-IN FUNCTIONS
int help(char **args)
{
    char *topic_str = args[1];
    int topic = -1;

    if (topic_str == NULL) {
        puts("Welcome to PIZZAshell :D\nTry using help with a tasty argument...");
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
            puts("Usage: help [\033[4mtopic\033[0m]");
            break;
        case TOPICS:
            printf("Help topics: ");
            for (int i = 0; i < topic_count; ++i)
                printf("%s ", help_topics[i]);
            printf("\n");
            break;
        case BUILTINS:
            puts("Available built-in functions:\ncd, help, info, math, echo, canal, hanal");
            break;
        case CD:
            puts("Usage: cd [-|\033[4mdirectory\033[0m]");
            break;
        case INFO:
            puts("Usage: info [\033[4mfilename\033[0m]");
            break;
        case MATH:
            puts("Usage: math [\033[4moperand\033[0m] [\033[4moperator\033[0m] [\033[4moperand\033[0m]");
            break;
        case ECHO:
            puts("Usage: echo [-n] [\033[4mstring\033[0m]");
            break;
        case PIZZA:
            puts("It is delicious. Need I say more?");
            break;
        case CANAL:
            puts("Analyze the current directory and all subdirectories. Prints lines of code for each file with extension '.c', '.h', or '.py'.");
            break;
        case HANAL:
            puts("Analyze the current directory and all subdirectories. Prints Halstead vocabulary for each file with extension '.c' '.h' or '.py'.\n\tn1 is count of distinct operators\n\tn2 is count of distinct operands.");
            break;
        default:
            puts("undefined topic");
            break;
    }

    return 0;
}

int cd(char **args)
{
    char *dest = args[1];

    if (dest == NULL) {
        dest = getenv("HOME");
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
        puts("Usage: info \033[4mfilename\033[0m");
        return 0;
    }

    struct stat info;
    if (lstat(fname, &info) < 0) { return -1; }

    struct passwd *u_info;
    u_info = getpwuid(info.st_uid);

    char *owner = u_info->pw_name;
    char *filetype;
    char perms[7];

    sprintf(perms, "%o", info.st_mode);

    switch (info.st_mode & S_IFMT) {
        case S_IFREG:
            filetype = "regular file";
            break;
        case S_IFDIR:
            filetype = "directory";
            break;
        case S_IFCHR:
            filetype = "character device";
            break;
        case S_IFBLK:
            filetype = "block device";
            break;
        case S_IFIFO:
            filetype = "named pipe";
            break;
        case S_IFLNK:
            filetype = "symbolic link";
            break;
        default:
            filetype = "unknown";
            break;
    }

    #ifdef __APPLE__
        printf(" File: %s\n Size: %lld B\nOwner: %s\n Type: %s\nPerms: %s\n",
                fname, info.st_size, owner, filetype, &perms[2]);
    #else
        printf(" File: %s\n Size: %lu B\nOwner: %s\n Type: %s\nPerms: %s\n",
                fname, info.st_size, owner, filetype, &perms[2]);
    #endif
    return 0;
}

int math(char **args)
{
    if (args[1] == NULL || args[2] == NULL || args[3] == NULL) {
        puts("Usage: math \033[4mterm1\033[0m \033[4moperator\033[0m \033[4mterm2\033[0m");
        return 0;
    }

    long t1 = strtol(args[1], NULL, 0);
    char op = args[2][0];
    long t2 = strtol(args[3], NULL, 0);

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
    if (args[1] == NULL) {
        puts("");
        return 0;
    }

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
        execlp("python3", "python3", "./complexity/Main.py", "-h", NULL);
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
