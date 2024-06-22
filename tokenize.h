#include "headers.h"

// void child_care_taker(int sigma);
// void fore_back_pro(char *tokens[], int tk_freq, int process_identity);
// void tokenize_first(char *first_tokenisable_string);
// int first_argument_processing(char *input, int process_identity);
extern struct ProcessInfo *processList;
struct ProcessInfo *addProcess(struct ProcessInfo *head, pid_t pid, char *tokens[], int sz);
struct ProcessInfo *findProcess(struct ProcessInfo *head, pid_t pidToFind);
struct ProcessInfo *removeProcess(struct ProcessInfo *head, pid_t pid);
void child_care_taker(int sigma);
void redirec_cmpq(char *token, char *file);
int redirection_handler(char *line, int processid, int pipes[], int prev, int flag);
void wait_n_switch(int child_pid);
void fg_handler(char **tokens, int n);
void bg_handler(char **tokens, int n);
// void fore_back_pro(char *tokens[], int tk_freq, int process_identity);
void function_call(char *line);
void check_input(int come, int leave);
void get_commands(char *line);
int first_argument_processing(char *tokens[], int tk_freq, int process_identity, int pipes[], int prev);
void pipechecker(char *command, int processid);
