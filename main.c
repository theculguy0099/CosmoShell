#include "headers.h"
#include "prompt.h"
#include "tokenize.h"
#define MAX 1000

char *current_direc;
char *undo;
char *original;
char *manjur;
int ok;
int flagger;
int file_hai;
int directory_hai;
char *to_change;
char *s;
char *command_pid[1000001];

void display_spaceship_banner() {
    printf("\n");
    printf("         _____\n");
    printf("     ,-:\"     \":-.\n");
    printf("    /,---.   ,---.\\\n");
    printf("   [(  )] ' [(  )]\n");
    printf("    \\____/   \\____/\n");
    printf("     ====     ====\n");
    printf("    /____\\   /____\\\n");
    printf("     |  |     |  |\n");
    printf("     |__|     |__|\n");
    printf("     /\"\"\\\     /\"\"\\\n");
    printf("    /____\\   /____\\\n");
    printf("\n");
    printf("Welcome to CosmoShell - Your Cosmic Command Center!\n\n");
}

int paster = 0;
void custom_sigint_handler(int signum)
{
    int targetPID = foregroundProcessPid;
    if ((foregroundProcessPid == -1))
        return;
    if (kill(targetPID, 9) == -1)
    {
        perror("kill");
    }
    foregroundProcessPid = -1;
}
int foregroundProcessPid = -1;

void sigtstp_handler(int signum)
{
    int targetPID = foregroundProcessPid;
    if ((foregroundProcessPid == -1))
        return;

    if (kill(targetPID, SIGSTOP) == -1)
    {

        perror("kill");
    }
    foregroundProcessPid = -1;
}
struct ProcessInfo *processList = NULL;

int main()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    if (signal(SIGINT, custom_sigint_handler) == SIG_ERR)
    {
        perror("signal");
        return 1;
    }
    if (signal(SIGTSTP, sigtstp_handler) == SIG_ERR)
    {
        perror("signal");
    }

    for (int i = 0; i < 1000001; i++)
    {
        command_pid[i] = (char *)malloc(sizeof(char) * MAX);
    }
    s = (char *)malloc(sizeof(char) * MAX);
    // Keep accepting commands
    printf("\033[H\033[J");
    // At first, the current directory is the home directory and here are various initializations.
    current_direc = (char *)malloc(sizeof(char) * MAX);
    undo = (char *)malloc(sizeof(char) * MAX);
    original = (char *)malloc(sizeof(char) * MAX);
    manjur = (char *)malloc(sizeof(char) * MAX);
    to_change = (char *)malloc(sizeof(char) * MAX);

    if (getcwd(current_direc, MAX) == NULL)
    {
        perror("getcwd failed");
        exit(EXIT_FAILURE);
    }
    strcpy(undo, current_direc);

    strcpy(original, current_direc);
    char *input = (char *)malloc(sizeof(char) * 4096);
    // int inp = dup(0);
    // int output = dup(1);
    int set=0;
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        // printf("kevin\n");
        if(!set)
        {
            set=1;
            display_spaceship_banner();
        }
        prompt();
        if (fgets(input, 4096, stdin) == NULL)
        {

            while (processList != NULL)
            {
                struct ProcessInfo *x = processList;
                processList = processList->next;
                // printf("%d ", x->pid);
                // for(int i=0;i<x->sz;i++){
                //     printf("%s ",x->name[i]);
                // }
                // printf("\n");
                if (kill(x->pid, 9) == 0)
                {
                    // printf("Signal %d sent to process with PID %d\n", 9, x->pid);
                }
                else
                {
                    perror("kill");
                    return 1;
                }
                // kill(x->pid, 9);
                processList = removeProcess(processList, x->pid);
            }

            printf("\n");
            exit(0);
        }

        /*
        The code iterates through a linked list of finished child processes.
        For each process, it determines whether the process exited abnormally due to a signal
         or normally with an exit status. Information is printed, the linked list is freed,
          and the list's pointer is reset.
        */
        node *recent_node = completedList;
        while (recent_node != NULL)
        {
            if (WIFSIGNALED(recent_node->process_status))
            {
                printf("Child process with PID %d exited abnormally due to signal %d\n", recent_node->pid, WTERMSIG(recent_node->process_status));
            }
            else if (WIFEXITED(recent_node->process_status))
            {
                printf("Child process with PID %d exited normally with process_status %d\n", recent_node->pid, WEXITSTATUS(recent_node->process_status));
            }
            node *res = recent_node;
            recent_node = recent_node->next;
            free(res);
        }
        completedList = NULL;

        // char *first_tokenisable_string = (char *)malloc(sizeof(char) * MAX);
        // strcpy(first_tokenisable_string, input);

        // Now the whole input string will go into several steps of tokenization.
        input = trimspace(input);
        // printf("neel\n");
        function_call(input);
        // printf("neel2\n");
    }
}
