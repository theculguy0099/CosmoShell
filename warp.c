#include "headers.h"

void substitute(char **string_1, char **string_2)
{
    char *res = *string_1;
    *string_1 = *string_2;
    *string_2 = res;
}

void warps(int pid, int tk_freq, char *tokens[])
{
    if (pid)
    {
        printf("[%d]\n", getpid());
    }
    // for (int i = 0; i < tk_freq; i++)
    // {
    //     printf("%s\n", tokens[i]);
    // }

    // return;
    /*
    The below code follows the change directory phase whenever necessary tokens
    are provided after the warp.
    */
    // printf("hello\n");
    if (tk_freq == 1)
    {
        if (chdir(original) == -1)
        {
            perror("chdir");
        }
        strcpy(current_direc, original);
        printf("%s\n", current_direc);
    }
    else
    {
        // different specifications start from here:
        for (int i = 1; i < tk_freq; i++)
        {
            tokens[i] = trimspace(tokens[i]);

            if (strcmp(tokens[i], "-") == 0)
            {
                if (chdir(undo) == -1)
                {
                    perror("chdir");
                }
                substitute(&current_direc, &undo);
                printf("%s\n", current_direc);
            }
            else if (strcmp(tokens[i], ".") == 0)
            {
                printf("%s\n", current_direc);
            }
            else if (strcmp(tokens[i], "") == 0)
            {
                if (chdir(original) == -1)
                {
                    perror("chdir");
                }
                strcpy(undo, current_direc);
                strcpy(current_direc, original);
                printf("%s\n", current_direc);
            }
            else if (strcmp(tokens[i], "~") == 0)
            {
                if (chdir(original) == -1)
                {
                    perror("chdir");
                }
                strcpy(undo, current_direc);
                strcpy(current_direc, original);
                printf("%s\n", current_direc);
            }
            else if (strcmp(tokens[i], " ") == 0)
            {
                if (chdir(original) == -1)
                {
                    perror("chdir");
                }
                strcpy(undo, current_direc);
                strcpy(current_direc, original);
                printf("%s\n", current_direc);
            }
            else
            {

                strcpy(undo, current_direc);
                if (chdir(tokens[i]) == -1)
                    perror("No such type of directory exists");
                else
                {
                    getcwd(current_direc, MAX);
                    printf("%s\n", current_direc);
                }
                // printf("yes\n");
                // return;

            }
        }
    }
}