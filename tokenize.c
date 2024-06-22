#include "tokenize.h"

char *warp = "warp";
char *peek = "peek";
char *pastevents = "pastevents";
char *seek = "seek";
char *proclore = "proclore";
char *purge = "purge";

void check_input(int in, int out)
{
    dup2(in, 0);
    close(in);
    dup2(out, 1);
    close(out);
}

void get_commands(char *line)
{
    char *command = (char *)malloc(sizeof(char) * MAX);
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
    if (numberofstore == 0)
        return;
    char *beg = copy1;
    store[0] = strtok(copy1, "&;");

    while (store[i] != NULL && strcmp(store[i], "") != 0)
    {
        i++, store[i] = strtok(NULL, "&;");
    }
    // int past = 0;
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
        int backup_stdout = dup(STDOUT_FILENO);
        int backup_stdin = dup(STDIN_FILENO);
        pipechecker(store[j], processid);
        check_input(backup_stdin, backup_stdout);
        // past += processInput(store[j], processid);
    }

    if (!paster && !ok)
    {

        // printf("%s\n",line);
        FILE *fptr;
        char *file = (char *)malloc(sizeof(char) * 1024);
        strcpy(file, original);
        strcat(file, "/history.txt");
        fptr = fopen(file, "r+");
        if (fptr == NULL)
        {
            printf("Error!");
            return;
        }
        char buffer[1024];
        char *lastLine[16];
        for (int i = 0; i < 16; i++)
        {
            lastLine[i] = (char *)malloc(sizeof(char) * 1024);
        }
        int ct = 0;
        while (fgets(buffer, sizeof(buffer), fptr) != NULL)
        {
            strcpy(lastLine[ct++], buffer);
        }

        if (strcmp(copy2, lastLine[ct - 1]) == 0)
        {
            fclose(fptr);
            return;
        }
        else
        {
            fclose(fptr);
            fptr = fopen(file, "w");

            if (ct < 15)
            {
                for (int i = 0; i < ct; i++)
                {
                    fprintf(fptr, "%s", lastLine[i]);
                }
                if (ct > 0)
                {
                    fprintf(fptr, "\n");
                }
                // printf("%d\n", ct);
                fprintf(fptr, "%s", line);
            }
            else
            {
                if (ftruncate(fileno(fptr), 0) != 0)
                {
                    perror("Error truncating file");
                    fclose(fptr);
                    return;
                }
                for (int i = 1; i < 15; i++)
                {
                    fprintf(fptr, "%s", lastLine[i]);
                }
                fprintf(fptr, "\n");
                fprintf(fptr, "%s", copy2);
            }
        }
        fclose(fptr);
    }
}

void pipechecker(char *command, int processid)
{
    int count = 0;
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] == '|')
        {
            count++;
        }
    }
    if (count == 0)
    {
        redirection_handler(command, processid, NULL, -1, 1);
        return;
    }
    int it = 0;
    char *commands[count + 10];
    char *x = strtok(command, "|");
    while (x != NULL)
    {
        commands[it++] = malloc(sizeof(char) * 1024);
        strcpy(commands[it - 1], x);
        x = strtok(NULL, "|");
    }

    int prev = 0;
    int output = dup(1);
    int input = dup(0);
    for (int i = 0; i <= count; i++)
    {
        commands[i] = trimspace(commands[i]);
    }
    int pipes[2];
    for (int i = 0; i < it; i++)
    {

        if (pipe(pipes) == -1)
        {
            return;
        }
        int pid = fork();
        if (pid == -1)
        {
            return;
        }
        else if (pid == 0)
        {
            dup2(prev, 0);
            if (i != it - 1)
            {
                dup2(pipes[1], 1);
            }
            close(pipes[0]);
            redirection_handler(commands[i], processid, pipes, prev, 0);
            exit(0);
        }
        else
        {
            wait(NULL);
            close(pipes[1]);
            prev = pipes[0];
        }
    }
    dup2(output, 1);
    dup2(input, 0);
    close(output);
    close(input);
    // dup2(output, 1);
    // if (prev != -1)
    // {
    //     dup2(prev, 0);
    //     pipes[0] = prev;
    //     redirection_handler(commands[it - 1], processid, pipes, prev, 1);
    //     close(prev);
    // }
    // else
    // {
    //     dup2(input, 0);
    //     redirection_handler(commands[it - 1], processid, NULL, prev, 1);
    // }
    // close(pipes[0]);
    // close(pipes[0]);

    // dup2(input, 0);
    // dup2(output, 1);
    return;
}

// Function to add a new process to the linked list
struct ProcessInfo *addProcess(struct ProcessInfo *head, pid_t pid, char *tokens[], int sz)
{
    struct ProcessInfo *newNode = (struct ProcessInfo *)malloc(sizeof(struct ProcessInfo));
    if (newNode == NULL)
    {
        perror("malloc");
        exit(1);
    }
    newNode->name = (char **)malloc(sizeof(char *) * (sz + 1));
    for (int i = 0; i < sz; i++)
    {
        newNode->name[i] = (char *)malloc(sizeof(char) * 1024);
        strcpy(newNode->name[i], tokens[i]);
    }
    newNode->sz = sz;
    newNode->pid = pid;
    newNode->next = head;
    return newNode;
}
struct ProcessInfo *findProcess(struct ProcessInfo *head, pid_t pidToFind)
{
    struct ProcessInfo *current = head;

    while (current != NULL)
    {
        if (current->pid == pidToFind)
        {
            // Process with the specified PID found
            return current;
        }
        current = current->next;
    }

    // Process with the specified PID not found
    return NULL;
}

struct ProcessInfo *removeProcess(struct ProcessInfo *head, pid_t pid)
{
    struct ProcessInfo *current = head;
    struct ProcessInfo *prev = NULL;

    while (current != NULL)
    {
        if (current->pid == pid)
        {
            if (prev == NULL)
            {
                head = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }

    return head;
}

node *completedList = NULL;

void child_care_taker(int signum)
{
    int status;
    pid_t completedPid;

    while ((completedPid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        node *newNode = (node *)malloc(sizeof(node));
        if (newNode == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        newNode->pid = completedPid;
        newNode->process_status = status;
        // newNode->name=name;
        // if (prctl(PR_GET_NAME, newNode->process_name) != 0)
        // {
        //     perror("Process name retrieval error");
        //     strncpy(newNode->process_name, "unknown", 100 - 1);
        // }
        strncpy(newNode->proc_name, s, 100 - 1);
        newNode->proc_name[100 - 1] = '\0'; // Ensure null-termination

        newNode->next = completedList;
        completedList = newNode;
    }
}

void redirec_cmpq(char *token, char *file)
{
    if (strcmp(token, ">") == 0)
    {
        int new_fd;
        if ((new_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
        {
            perror("cannot redirect output");
        }
        else
        {
            dup2(new_fd, STDOUT_FILENO);
            close(new_fd);
        }
    }
    else if (strcmp(token, ">>") == 0)
    {
        int new_fd;
        if ((new_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1)
        {
            perror("cannot redirect output");
        }
        else
        {
            dup2(new_fd, STDOUT_FILENO);
            close(new_fd);
        }
    }
    else if (strcmp(token, "<") == 0)
    {
        int new_fd;
        if ((new_fd = open(file, O_RDONLY)) == -1)
        {
            perror("No such input file found!");
            return;
        }
        else
        {
            dup2(new_fd, STDIN_FILENO);
            close(new_fd);
        }
    }
}

int redirection_handler(char *line, int proccessid, int pipes[], int prev, int flag)
{
    char copy1[1024], copy2[1024];
    strcpy(copy1, line);
    strcpy(copy2, line);
    char *tokens[1000];
    int num_tokens = 0;
    tokens[0] = strtok(copy2, " \t\n");
    while (tokens[num_tokens] != NULL)
    {
        tokens[++num_tokens] = strtok(NULL, " \t");
    }
    char *store[100];
    for (int i = 0; i < 100; i++)
    {
        store[i] = (char *)malloc(sizeof(char) * 1024);
    }
    // int input = dup(0);
    // int output = dup(1);
    // int cunts[num_tokens];
    int ct = 0;
    int f = 0;
    for (int i = 0; i < num_tokens; i++)
    {
        if (strcmp(">>", tokens[i]) == 0)
        {
            f = 1;
            redirec_cmpq(tokens[i], tokens[i + 1]);
        }
        else if (strcmp(">", tokens[i]) == 0)
        {
            f = 1;
            redirec_cmpq(tokens[i], tokens[i + 1]);
        }
        else if (strcmp("<", tokens[i]) == 0)
        {
            f = 1;
            redirec_cmpq(tokens[i], tokens[i + 1]);
        }
        else if (f == 0)
        {
            strcpy(store[ct++], tokens[i]);
        }
    }
    first_argument_processing(store, ct, proccessid, pipes, prev);

    return 0;
}

void wait_n_switch(int child_pid)
{
    // signal(SIGTTIN, SIG_IGN);
    // tcsetpgrp(STDIN_FILENO, child_pid);
    kill(child_pid, SIGCONT);
    int status;
    foregroundProcessPid = child_pid;
    waitpid(child_pid, &status, WUNTRACED);
    // printf("GOOD\n");
    // printf("%d", tcsetpgrp(STDIN_FILENO, getpgid(getpid())));
    // return;

    /*   if (tcsetpgrp(STDOUT_FILENO, getpgid(getpid())) == -1) {
           //perror("cant get terminal back");
       }*/
    if (tcsetpgrp(STDIN_FILENO, getpgid(getpid())) == -1)
    {
        perror("cant get terminal back");
    }
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    if (WIFEXITED(status))
    {
        processList = removeProcess(processList, child_pid);
        // exit_code = WEXITSTATUS(status);
    }
    else if (WIFSTOPPED(status))
    {
        printf("\n");
        struct ProcessInfo *x = findProcess(processList, child_pid);
        printf(" %d ", x->pid);
        for (int i = 0; i < x->sz; i++)
        {
            printf("%s ", x->name[i]);
        }
        printf("Stopped\n");
    }
    else if (WIFSIGNALED(status))
    {
        processList = removeProcess(processList, child_pid);
    }
}
void fg_handler(char **tokens, int n)
{
    if (n != 2)
    {
        fprintf(stderr, "fg: invalid format! correct format is fg <job number>\n");
        // exit_code = 1;
        return;
    }
    int t = (int)strtol(tokens[1], NULL, 10);
    int pid = t;
    if (pid <= 0)
    {
        fprintf(stderr, "Job does not exist \n");
        // exit_code = 1;

        return;
    }
    wait_n_switch(pid);
}
void bg_handler(char **tokens, int n)
{
    if (n != 2)
    {
        fprintf(stderr, "bg: invalid format! bg <job number>\n");
        // exit_code = 1;
        return;
    }
    int t = (int)strtol(tokens[1], NULL, 10);
    int pid = t;
    if (pid <= 0)
    {
        // exit_code = 1;
        fprintf(stderr, "Job does not exist \n");
        return;
    }

    if (kill(pid, SIGCONT) == -1)
    {
        // exit_code = 1;
    }
}

int first_argument_processing(char *tokens[], int num_tokens, int proccessid, int pipes[], int prev)
{

    if (strcmp(tokens[0], warp) == 0)
    {
        paster = 0;
        printf("🚀 Initiating warp drive...\n");
        warps(proccessid, num_tokens, tokens);
    }
    else if (strcmp(tokens[0], peek) == 0)
    {
        paster = 0;
        printf("🔭 Activating long-range scanners...\n");
        peeks(tokens, num_tokens, proccessid);
    }
    else if (strcmp(tokens[0], pastevents) == 0)
    {
        paster = 1;
        printf("📜 Accessing the cosmic chronicles...\n");
        return pasts(tokens, num_tokens, proccessid);
        // paster = 0;
    }
    else if (strcmp(tokens[0], seek) == 0)
    {
        paster = 0;
        printf("🔍 Scanning the cosmic archives...\n");
        seeks(tokens, num_tokens);
    }
    else if (strcmp(tokens[0], proclore) == 0)
    {
        paster = 0;
        printf("📊 Analyzing ship's systems...\n");
        if (num_tokens == 1)
        {
            proclore_function(getpid());
        }
        else
            proclore_function(atoi(tokens[1]));
    }
    else if (strcmp(tokens[0], "finish") == 0)
    {
        paster = 0;
        printf("🌠 Preparing for hyperspace jump...\n");
        printf("\033[1;31mOh!\nYou are out of your CosmoShell now!\n\033[0m");
        exit(1);
    }
    else if (strcmp(tokens[0], "ping") == 0)
    {
        paster = 0;
        printf("📡 Sending subspace signal...\n");
        int signals = atoi(tokens[2]);
        int pid = atoi(tokens[1]);
        signals %= 32;

        if (kill(pid, signals) == 0)
        {
            printf("Signal %d sent to process with PID %d\n", signals, pid);
        }
        else
        {
            perror("kill");
            return 1;
        }
    }
    else if (strcmp(tokens[0], "activities") == 0)
    {
        paster = 0;
        printf("👨‍🚀 Checking crew activities...\n");
        struct ProcessInfo *current = processList;
        // struct ProcessInfo *prev = NULL;
        while (current != NULL)
        {
            int pid = current->pid;
            // int job = (pid);
            char location[1024];
            sprintf(location, "/proc/%d/cmdline", pid);
            sprintf(location, "/proc/%d/stat", pid);
            FILE *f = fopen(location, "r");
            char state = 0;
            int pd;
            char new_name[1024];
            char statee[100];
            if (f == NULL)
            {
                printf(" %d ", current->pid);
                for (int i = 0; i < current->sz; i++)
                {
                    printf("%s ", current->name[i]);
                }

                printf("finished\n");
                processList = removeProcess(processList, current->pid);
            }
            else
            {
                fscanf(f, " %d ", &pd);
                fscanf(f, " %s ", new_name);
                fscanf(f, " %c ", &state);
                if (state == 'R')
                    strcpy(statee, "Running");
                else if (state == 'T')
                    strcpy(statee, "Stopped");
                else if (state == 'Z')
                    strcpy(statee, "Zombie");
                else if (state == 'S')
                    strcpy(statee, "Running");
                else
                    strcpy(statee, "Unknown State");
                int flag = 0;
                if (f == NULL)
                {
                    flag = 1;
                }
                fclose(f);
                printf(" %d ", current->pid);
                for (int i = 0; i < current->sz; i++)
                {
                    printf("%s ", current->name[i]);
                }
                printf("%s\n", statee);
            }

            // prev = current;
            current = current->next;
        }
    }
    else if (strcmp(tokens[0], "fg") == 0)
    {
        paster = 0;
        printf("🕹️ Bringing process to main viewscreen...\n");
        fg_handler(tokens, num_tokens);
    }
    else if (strcmp(tokens[0], "bg") == 0)
    {
        paster = 0;
        printf("💫 Sending process to background starfield...\n");
        bg_handler(tokens, num_tokens);
    }
    else if (strcmp(tokens[0], "iMan") == 0)
    {
        paster = 0;
        printf("📚 Accessing intergalactic manual...\n");
        if (num_tokens == 1)
        {
            perror("invaild command");
            return 0;
        }
        fetchManPage(tokens[1]);
    }
    else if (strcmp(tokens[0], "neonate") == 0)
    {
        paster = 0;
        printf("👶 Monitoring newborn star formation...\n");
        neonate(tokens);
    }
    else
    {
        printf("🖥️ Interfacing with starship mainframe: %s\n", tokens[0]);
        char *command = strdup(tokens[0]);
        char *argv[num_tokens + 1];
        for (int i = 0; i < num_tokens; i++)
        {
            argv[i] = strdup(tokens[i]);
        }
        if (argv[num_tokens - 1][strlen(argv[num_tokens - 1]) - 1] == '\n')
            argv[num_tokens - 1][strlen(argv[num_tokens - 1]) - 1] = '\0';
        argv[num_tokens] = NULL;

        time_t start_time, end_time;
        start_time = time(NULL);

        int rc = fork();
        if (!proccessid)
            foregroundProcessPid = rc;
        if (rc < 0)
            perror("creating child process failed\n");
        else if (rc == 0)
        {

            if (proccessid)
                setpgid(0, 0);
            if (execvp(command, argv) == -1)
            {
                ok = 1;
                printf("invalid command\n");
                exit(1);
            }
        }
        else if (rc > 0)
        {

            if (!proccessid)
                waitpid(rc, NULL, WUNTRACED);
            else
                printf("child with pid [%d] sent to background\n", rc);
            strcpy(s, tokens[0]);
            strcpy(command_pid[rc], tokens[0]);
            processList = addProcess(processList, rc, tokens, num_tokens);
            signal(SIGCHLD, child_care_taker);
            end_time = time(NULL);
            int duration = (int)(end_time - start_time);
            if (duration > 2 && !proccessid)
            {
                flagger = duration;
                strcpy(manjur, command);
            }
        }
    }
    return 0;
}

void function_call(char *line)
{
    char *token = (char *)malloc(sizeof(char) * 1024);
    strcpy(token, line);

    token = trimspace(token);
    // printf("%s\n",token);
    // printf("hello\n");
    get_commands(token);
    // printf("HELLO\n");
}