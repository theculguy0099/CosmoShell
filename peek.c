#include "headers.h"
// Comparator function for qsort
struct stat data;

int cmpstringp(const void *p1, const void *p2)
{
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}
char *months_data(int index, char *naam);

void my_print(struct stat *data, int freq, char *concent, char **mega, int i, int flag1, int flag2, int flag3)
{
    if (flag3)
    {
        printf("%s", "\033[1m\033[33m");
    }
    else if (flag1)
    {
        printf("%s", "\033[1m\033[34m");
    }
    else if (flag2)
    {
        printf("%s", "\033[1m\033[32m");
    }
    else
    {
        printf("%s", "\033[1m\033[0m");
    }
    struct passwd *password = getpwuid(data->st_uid);
    struct group *together = getgrgid(data->st_gid);

    change(data->st_mode, concent, freq);

    time_t t = data->st_mtime;
    struct tm modes;
    localtime_r(&t, &modes);
    int month = modes.tm_mon;

    char *mahina = (char *)malloc(sizeof(char) * 5);
    months_data(month, mahina);

    printf("%s ", concent);
    printf("%2ld ", data->st_nlink);
    printf("%s  %s ", password->pw_name, together->gr_name);
    printf("%5ld ", data->st_size);
    printf("%s %02d %02d:%02d ", mahina, modes.tm_mday, modes.tm_hour, modes.tm_min);
    printf("%s ", mega[i]);
    printf("%s\n", "\033[1m\033[0m");
}
void my_next_print(int flag1, int flag2, int flag3, char **mega, int i)
{
    if (flag3)
    {
        printf("%s", "\033[1m\033[33m");
    }
    else if (flag1)
    {
        printf("%s", "\033[1m\033[34m");
    }
    else if (flag2)
    {
        printf("%s", "\033[1m\033[32m");
    }
    else
    {
        printf("%s", "\033[1m\033[0m");
    }
    printf("%s\n", mega[i]);
    printf("%s", "\033[1m\033[0m");
}

void check_timeline(char *dir, char **mega, int agar_a, int agar_l, int i)
{

    char *form_direc = (char *)malloc(sizeof(char) * MAX);
    strcpy(form_direc, dir);
    strcat(form_direc, "/");
    strcat(form_direc, mega[i]);
    if (lstat(form_direc, &data) == -1)
    {
        printf("%s\n", mega[i]);
        perror("Error getting stat struct");
        return;
    }
    char concent[100];
    int freq = 0;
    // char var[100];
    int flag1 = 0;
    int flag2 = 0;
    int flag3 = 0;
    if (data.st_mode & __S_IFDIR)
    {
        concent[freq++] = 'd';
        flag1 = 1;
    }
    else if (data.st_mode & S_IXUSR)
    {
        flag2 = 1;
        concent[freq++] = '-';
    }
    else if (S_ISLNK(data.st_mode))
    {
        flag3 = 1;

        concent[freq++] = 'l';
    }
    else
    {
        concent[freq++] = '-';
    }
    if (agar_l)
    {
        my_print(&data, freq, concent, mega, i, flag1, flag2, flag3);
    }
    else
    {
        my_next_print(flag1, flag2, flag3, mega, i);
    }
}

void process_further(char *dir, int agar_a, int agar_l);

void check_condition_1(char *tokens[], int tk_freq, int hide, int information)
{

    if (strcmp(tokens[1], "-a") == 0)
    {
        // printf("main -a me pahuch gaya\n");
        process_further(".", 1, 0);
    }

    else if (strcmp(tokens[1], "-l") == 0)
    {
        process_further(".", 0, 1);
    }
    else if (strcmp(tokens[1], "."))
    {
        process_further(tokens[1], hide, information);
    }
    else if (strcmp(tokens[1], ".."))
    {
        process_further(tokens[1], hide, information);
    }
    else if (strcmp(tokens[1], "~"))
    {
        process_further(tokens[1], hide, information);
    }
    else if (strcmp(tokens[1], "-"))
    {
        process_further(tokens[1], hide, information);
    }
    else
    {
        process_further(tokens[1], hide, information);
    }
}
void check_condition_3(char *tokens[], int hide, int information)
{
    hide = 1;
    information = 1;
    process_further(tokens[3], hide, information);
}
void check_condition_2(char *tokens[], int hide, int information)
{
    if (strlen(tokens[1]) == 2)
    {
        if (tokens[1][1] == 'a')
        {
            int kt = 1;
            hide = kt;
        }
        else if (tokens[1][1] == 'l')
        {
            int ktt = 1;
            information = ktt;
        }
    }
    else if (strlen(tokens[1]) == 3)
    {
        if (tokens[1][1] == 'a')
        {
            int kt = 1;
            hide = kt;
        }
        else if (tokens[1][1] == 'l')
        {
            int ktt = 1;
            information = ktt;
        }
        if (tokens[1][2] == 'a')
        {
            int tk = 1;
            hide = tk;
        }
        else if (tokens[1][2] == 'l')
        {
            int tkk = 1;
            information = tkk;
        }
    }
    process_further(tokens[2], hide, information);
}
void peeks(char *tokens[], int tk_freq, int process_identity)
{
    if (process_identity)
    {
        printf("[%d]\n", getpid());
    }
    int information = 0;
    int hide = 0;
    if (tk_freq == 3)
    {
        check_condition_2(tokens, hide, information);
    }
    else if (tk_freq == 4)
    {
        check_condition_3(tokens, hide, information);
    }
    else if (tk_freq == 2)
    {
        check_condition_1(tokens, tk_freq, hide, information);
    }
}

char *months_data(int index, char *naam)
{
    switch (index)
    {
    case 0:
        strcpy(naam, "Jan");
        break;
    case 1:
        strcpy(naam, "Feb");
        break;
    case 2:
        strcpy(naam, "Mar");
        break;
    case 3:
        strcpy(naam, "Apr");
        break;
    case 4:
        strcpy(naam, "May");
        break;
    case 5:
        strcpy(naam, "Jun");
        break;
    case 6:
        strcpy(naam, "Jul");
        break;
    case 7:
        strcpy(naam, "Aug");
        break;
    case 8:
        strcpy(naam, "Sep");
        break;
    case 9:
        strcpy(naam, "Oct");
        break;
    case 10:
        strcpy(naam, "Nov");
        break;
    case 11:
        strcpy(naam, "Dec");
        break;
    default:
        break;
    }

    return naam;
}

void process_further(char *dir, int agar_a, int agar_l)
{
    if (strcmp(dir, "-") == 0)
    {
        strcpy(dir, original);
    }
    else if (strcmp(dir, "~") == 0)
    {
        strcpy(dir, undo);
    }

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
        // exit(EXIT_FAILURE);
        return;
    }

    int ct_of_files = 0;
    while (readdir(dh) != NULL)
    {
        ct_of_files++;
    }

    closedir(dh);
    dh = opendir(dir);
    char **mega = (char **)malloc(sizeof(char *) * ct_of_files);

    int ma = 0;
    while (ct_of_files--)
    {
        mega[ma++] = (char *)malloc(sizeof(char) * MAX);
    }

    int freq = 0;
    struct dirent *d;
    int amount = 0;
    while ((d = readdir(dh)) != NULL)
    {
        if (!agar_a && d->d_name[0] == '.')
        {
            continue;
        }
        strcpy(mega[freq++], d->d_name);
        struct stat data;
        char *form_direc = (char *)malloc(sizeof(char) * MAX);
        strcpy(form_direc, dir);
        strcat(form_direc, "/");
        strcat(form_direc, d->d_name);
        if (lstat(form_direc, &data) == -1)
        {
            printf("%s\n", d->d_name);
            perror("Error getting stat struct");
            return;
        }
        amount += data.st_blocks;
    }
    int half = amount / 2;
    if (agar_l)
    {
        printf("total %d\n", half);
    }

    qsort(mega, freq, sizeof(char *), cmpstringp);

    for (int i = 0; i < freq; i++)
    {
        check_timeline(dir, mega, agar_a, agar_l, i);

        free(mega[i]);
    }
    closedir(dh);
    free(d);
}

char *change(int first_number, char *var, int next_number)
{
    int freq = next_number;
    for (int i = 0; i < 3; i++)
    {
        int kachra = i * 3;
        int res = first_number >> (kachra);
        for (int j = 0; j < 3; j++)
        {

            if (((res >> j) & 1) != 1)
            {
                var[freq++] = '-';
            }
            else
            {
                if (j == 0)
                {
                    var[freq++] = 'x';
                }
                else if (j == 2)
                {
                    var[freq++] = 'r';
                }
                else if (j == 1)
                {
                    var[freq++] = 'w';
                }
            }
        }
    }
    int down = freq - 1;
    for (int i = 1; i < freq / 2 + 1; i++)
    {
        char res = var[i];
        var[i] = var[down - i];
        var[down - i] = res;
    }

    char res_new = var[down];

    for (int i = down; i > 1; i--)
    {
        var[i] = var[i - 1];
    }
    var[1] = res_new;
    var[freq] = '\0';
    return var;
}
