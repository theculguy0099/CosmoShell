#include "neonate.h"

void neonate(char **str)
{
    char chr;
    int count = 0;

    struct termios original_termios;
    tcgetattr(STDIN_FILENO, &original_termios);
    struct termios raw_termios = original_termios;
    raw_termios.c_lflag &= ~(ICANON | ECHO);
    raw_termios.c_cc[VMIN] = 0;
    raw_termios.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios);

    time_t start_time;
    time_t end_time;
    time(&start_time);
    
    while (1)
    {
        time(&end_time);
        double spent_time = difftime(end_time, start_time);

        if (spent_time > (count * atoi(str[2])))
        {
            count++;
            FILE *fptr = fopen("/proc/loadavg", "r");
            if (fptr == NULL)
            {
                fprintf(stderr, "Can't open /proc/loadavg file\n");
                break;
            }
            char info[1024];
            fgets(info, 1024, fptr);
            char *value = strtok(info, " ");
            for (int i = 0; i < 4; i++)
                value = strtok(NULL, " ");

            printf("%s", value);
        }

        if (read(STDIN_FILENO, &chr, 1) == -1)
        {
            perror("read");
            break;
        }
        if (chr == 'x')
        {
            chr = ' ';
            break;
        }
    }

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
    return;
}