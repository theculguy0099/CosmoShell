#include "pastevents.h"

void get_commands2(char *line)
{
    char *command;
    char copy1[1024], copy2[1024];
    strcpy(copy1, line);
    strcpy(copy2, line);
    command = strtok(line, "&;");
    int numberofstore = 0;
    while (command != NULL)
    {
        command = trimspace(command);
        numberofstore++, command = strtok(NULL, "&;");
    }

    char *store[numberofstore + 1];
    int i = 0;
    if (numberofstore <= 0)
        return;
    char *beg = copy1;
    store[0] = strtok(copy1, "&;");

    while (store[i] != NULL && strcmp(store[i], "") != 0)
    {
        i++;
        store[i] = strtok(NULL, "&;");
    }
    // int past = 0;
    int backup_stdout = dup(STDOUT_FILENO);
    int backup_stdin = dup(STDIN_FILENO);
    for (int j = 0; j < numberofstore; j++)
    {
        int processid = 0;
        if (copy2[strlen(store[j]) + (store[j] - beg)] == '&')
            processid = 1;
        if (strcmp(store[j], "\n") == 0)
        {
            printf("%s", (store[j]));
            continue;
        }
        // past += processInput(store[j], processid);
        pipechecker(store[j], processid);
        check_input(backup_stdin, backup_stdout);
    }
}

int pasts(char *tokens[], int tk_freq, int process_identity)
{
    if (process_identity)
    {
        printf("[%d]\n", getpid());
    }

    FILE *file_descriptor;
    char *file_name = (char *)malloc(sizeof(char) * MAX);
    strcpy(file_name, original);

    char *concat = "/history.txt";
    strcat(file_name, concat);

    file_descriptor = fopen(file_name, "r+");
    // Now checking various conditions according to the specified specifications!
    if (file_descriptor != NULL)
    {
        if (tk_freq == 1)
        {
            char end_line[MAX] = "";
            char dummy[MAX];

            while (fgets(dummy, sizeof(dummy), file_descriptor) != NULL)
            {
                printf("%s", end_line);
                strcpy(end_line, dummy);
            }
        }
        else if (tk_freq == 3)
        {
            if (strcmp(tokens[1], "execute") == 0)
            {

                char *end_line[16];
                char dummy[MAX];
                for (int i = 0; i < 16; i++)
                {
                    end_line[i] = (char *)malloc(sizeof(char) * MAX);
                }
                int ct = 0;
                while (fgets(dummy, sizeof(dummy), file_descriptor) != NULL)
                {
                    strcpy(end_line[ct++], dummy);
                    // printf("%s", end_line[ct - 1]);
                }
                // Now converting the string tokens[2] to integer and then executing the command.
                printf("%s", end_line[ct - (atoi(tokens[2]))]);

                get_commands2(end_line[ct - (atoi(tokens[2]))]);
            }
        }
        else if (tk_freq == 2)
        {
            if (strcmp(tokens[1], "purge") == 0)
            {
                if (ftruncate(fileno(file_descriptor), 0) != 0)
                {
                    perror("Error in file truncation");
                    fclose(file_descriptor);
                    return 1;
                }
            }
        }

        fclose(file_descriptor);
    }
    else
    {
        printf("Error in opening the file, check the file_descriptor!");
    }

    return 1;
}