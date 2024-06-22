#include "headers.h"

void rmv(char *filename)
{
    char *my = strrchr(filename, '.');
    if (my != NULL)
    {
        *my = '\0';
    }
}

int recursion(char *dir, int direc, int executable, int files, char *file);

void condition_1(char *tokens[])
{
    if (!recursion(current_direc, 1, 0, 1, tokens[1]))
    {
        printf("No match found!\n");
    }

    directory_hai = 0;
    file_hai = 0;
    return;
}
void condition_2(char *tokens[], int tk_freq)
{
    if (recursion(tokens[tk_freq - 1], 1, 0, 1, tokens[tk_freq - 2]))
    {
    }
    else
    {
        printf("No match found!\n");
    }
    directory_hai = 0;
    file_hai = 0;
    return;
}
void condition_3(char *tokens[], int tk_freq)
{
    if (recursion(tokens[tk_freq - 1], 1, 0, 1, tokens[tk_freq - 2]))
    {
    }
    else
    {
        printf("No match found!\n");
    }
    directory_hai = 0;
    file_hai = 0;
    return;
}
void condition_4(char *tokens[], int executable, int direc, int files)
{
    if (strcmp("-e", tokens[1]) == 0)
    {
        int van = 1;
        executable = van;
    }
    else if (strcmp("-d", tokens[1]) == 0)
    {
        int fg = 1;
        direc = fg;
    }
    else if (strcmp("-f", tokens[1]) == 0)
    {
        int ty = 1;
        files = ty;
    }
    else
    {
        printf("Invalid Command!\n");
        return;
    }
}
void condition_5(char *tokens[], int executable, int direc, int files)
{
    if (strcmp("-e", tokens[1]) == 0)
    {
        int mom = 1;
        executable = mom;
    }
    else if (strcmp("-d", tokens[1]) == 0)
    {
        int sik = 1;
        direc = sik;
    }
    else if (strcmp("-f", tokens[1]) == 0)
    {
        int mass = 1;
        files = mass;
    }
    else
    {
        printf("Invalid Command!\n");
        return;
    }
    if (strcmp("-e", tokens[2]) == 0)
    {
        int damn = 1;
        executable = damn;
    }
    else if (strcmp("-d", tokens[2]) == 0)
    {
        int dem = 1;
        direc = dem;
    }
    else if (strcmp("-f", tokens[2]) == 0)
    {
        int you = 1;
        files = you;
    }
    else
    {
        printf("Invalid Command!\n");
        return;
    }
    if (direc && files)
    {
        directory_hai = 0, file_hai = 0;
        printf("Flags are invalid !\n");
        return;
    }
}
void seeks(char *tokens[], int tk_freq)
{
    int executable = 0;
    int files = 0;
    int direc = 0;
    if (tk_freq - 2 == 0)
    {
        if (!recursion(current_direc, 1, 0, 1, tokens[1]))
        {
            printf("No match found!\n");
        }

        directory_hai = 0;
        file_hai = 0;
        return;
    }
    else if (tk_freq - 3 == 0)
    {
        if (recursion(tokens[tk_freq - 1], 1, 0, 1, tokens[tk_freq - 2]))
        {
        }
        else
        {
            printf("No match found!\n");
        }
        directory_hai = 0;
        file_hai = 0;
        return;
    }
    else if (tk_freq - 4 == 0)
    {
        if (strcmp("-e", tokens[1]) == 0)
        {
            int wow = 1;
            executable = wow;
        }
        else if (strcmp("-f", tokens[1]) == 0)
        {
            int wowo = 1;
            files = wowo;
        }
        else if (strcmp("-d", tokens[1]) == 0)
        {
            int wp = 1;
            direc = wp;
        }
        else
        {
            printf("Invalid Command!\n");
            return;
        }
    }
    else if (tk_freq - 5 == 0)
    {
        if (strcmp("-e", tokens[1]) == 0)
        {
            int sp = 1;
            executable = sp;
        }
        else if (strcmp("-f", tokens[1]) == 0)
        {
            int fifo = 1;
            files = fifo;
        }
        else if (strcmp("-d", tokens[1]) == 0)
        {
            int upi = 1;
            direc = upi;
        }
        else
        {
            printf("Invalid Command!\n");
            return;
        }
        if (strcmp("-e", tokens[2]) == 0)
        {
            int kik = 1;
            executable = kik;
        }
        else if (strcmp("-f", tokens[2]) == 0)
        {
            int koke = 1;
            files = koke;
        }
        else if (strcmp("-d", tokens[2]) == 0)
        {
            int sumo = 1;
            direc = sumo;
        }
        else
        {
            printf("Invalid Command!\n");
            return;
        }
        if (direc && files)
        {
            directory_hai = 0, file_hai = 0;
            printf("Flags are invalid !\n");
            return;
        }
    }
    else
    {
        printf("Invalid Command !\n");
        return;
    }
    if (files == 0 && direc == 0)
    {
        direc = 1;
        files = 1;
    }
    int c = recursion(tokens[tk_freq - 1], direc, executable, files, tokens[tk_freq - 2]);

    if (c == 0)
    {
        printf("No match found!\n");
    }
    else if (c == 1 && file_hai && !directory_hai && executable)
    {
        printf("%s\n", to_change);
        FILE *file_descriptor = fopen(to_change, "r");
        if (file_descriptor == NULL)
        {
            if (errno == EACCES)
            {
                printf("Task Permissions missing!\n");
            }
            printf("File can't be opened \n");
            exit(0);
        }
        c = fgetc(file_descriptor);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file_descriptor);
        }
        printf("\n");
        fclose(file_descriptor);
    }
    else if (c == 1 && directory_hai && !file_hai && executable)
    {
        chdir(to_change);
    }

    directory_hai = 0, file_hai = 0;
}

int recursion(char *dir, int direc, int executable, int files, char *file)
{
    DIR *dh = opendir(dir);
    if (!dh)
    {
        if (errno = ENOENT)
        {
            perror("Directory doesn't exist");
        }
        else
        {
            perror("Unable to read directory");
        }
        return 0;
    }
    struct dirent *d;
    int sum = 0;
    while ((d = readdir(dh)) != NULL)
    {
        char *temp = (char *)malloc(sizeof(char) * MAX);
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
        {
            continue;
        }
        strcpy(temp, d->d_name);
        rmv(temp);
        char path[MAX];
        snprintf(path, sizeof(path), "%s/%s", dir, d->d_name);
        struct stat statbuf;
        if (stat(path, &statbuf) == -1)
        {
            // perror("Error getting file information");
            continue;
        }
        if (S_ISDIR(statbuf.st_mode))
        {
            if (strcmp(temp, file) == 0 || strcmp(d->d_name, file) == 0)
            {
                sum++;
                directory_hai = 1;
                if (direc)
                {
                    printf("\033[1;32m%s\033[0m", path);
                    printf("\n");
                }
                strcpy(to_change, path);
            }
            sum += recursion(path, direc, executable, files, file);
        }
        else if (S_ISREG(statbuf.st_mode) && (strcmp(temp, file) == 0 || strcmp(d->d_name, file) == 0))
        {
            sum++;
            file_hai = 1;
            if (files)
            {
                printf("\033[1;34m%s\033[0m", path);
                printf("\n");
                strcpy(to_change, path);
            }
        }
    }
    closedir(dh);
    return sum;
}