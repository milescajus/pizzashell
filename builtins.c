#include "shell.h"

time_t rawtime;

// LIST OF BUILT-INS
int (*builtins[]) (char **) = {&cd, &help, &info, &math, &echo};
char *builtin_names[] = {"cd", "help", "info", "math", "echo"};

// HELP TOPICS
enum topics {HELP, TOPICS, BUILTINS, CD, INFO, MATH, ECHO, PIZZA};
char *help_topics[] = {"help", "topics", "builtins", "cd", "info", "math", "echo", "pizza"};
int topic_count = 8;

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
            puts("Help topics: ");
            for (int i = 0; i < topic_count; ++i)
                printf("%s ", help_topics[i]);
            puts("");
            break;
        case BUILTINS:
            puts("Available built-in functions:");
            for (int i = 0; i < builtin_count; ++i)
                printf("%s ", builtin_names[i]);
            puts("");
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

int update_time()
{
    rawtime = time(NULL);
    if(!strftime(time_str, 9, "%T", localtime(&rawtime)))
        return -1;

    return 0;
}

char *build_prompt()
{
    char *prompt_str;
    char *err_str;

    if (asprintf(&prompt_str, "\n\033[31;1m%s\033[0m [%s]", pwd, time_str) < 0) {
        perror("prompt: ");
        exit(1);
    }

    if (asprintf(&err_str, " C: \033[91m%d\033[0m", exit_status) < 0) {
        perror("prompt: ");
        exit(1);
    }

    if (exit_status)
        strcat(prompt_str, err_str);
    strcat(prompt_str, "\n🍕\033[38;5;220m$\033[0m ");

    free(err_str);
    return prompt_str;
}
