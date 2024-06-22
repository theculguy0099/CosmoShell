#include "headers.h"

int cmpstringp(const void *p1, const void *p2);
char *months_data(int index, char *naam);
void my_print(struct stat *data, int freq, char *concent, char **mega, int i, int flag1, int flag2, int flag3);
void my_next_print(int flag1, int flag2, int flag3, char **mega, int i);
void check_timeline(char *dir, char **mega, int agar_a, int agar_l, int i);
void process_further(char *dir, int agar_a, int agar_l);
void check_condition_1(char *tokens[], int tk_freq, int hide, int information);
void check_condition_3(char *tokens[], int hide, int information);
void check_condition_2(char *tokens[], int hide, int information);
void peeks(char *tokens[], int tk_freq, int process_identity);
char *change(int first_number, char *var, int next_number);
