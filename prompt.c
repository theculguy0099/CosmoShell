#include "headers.h"

// A function for getting the username using the library function getpwuid()
void get_username(char *username)
{
    uid_t uid = getuid();
    struct passwd *userInfo = getpwuid(uid);

    if (userInfo != NULL)
    {
        strcpy(username, userInfo->pw_name);
    }
    else
    {
        perror("getpwuid");
    }
}

void prompt()
{
    // Regular Prompt Building and Printing
    char *username = (char *)malloc(sizeof(char) * MAX);
    char now[MAX];
    char hostname[MAX];
    gethostname(hostname, MAX);
    getcwd(now, MAX);
    if (getcwd(now, MAX) == NULL)
    {
        perror("getcwd failed");
        exit(EXIT_FAILURE);
    }
    get_username(username);
    printf("\033[1;32m%s@\033[0m", username);
    printf("\033[1;33m%s\033[0m", hostname);
    if (strlen(now) < strlen(original))
    {
        printf(":\033[1;34m%s\033[0m", now);
        if (flagger > 2)
        {
            printf(" %s : %d ", manjur, flagger);
        }
        flagger = 0;
    }
    else
    {
        printf(":");
        printf("\033[1;34m~\033[0m");
        int kya = 2;
        if (flagger > kya)
        {
            printf("%s : %d ", manjur, flagger);
        }
        for (int i = strlen(original); i < strlen(now); i++)
        {
            printf("\033[1;34m%c\033[0m", now[i]);
        }
        flagger = 0;
    }
    printf(">");

    return;
}
