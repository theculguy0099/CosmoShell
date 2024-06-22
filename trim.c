#include "trim.h"

char *trimspace(char *string)
{
    int t = 0;
    int x = strlen(string);
    for (int i = 0; i < x; i++)
    {
        if (string[i] == ' ' || string[i] == '\t' || string[i] == '\n')
           t++;
        else
            break;
    }
    string += t;
    while(x--)
    {
        if (string[x] == ' ' || string[x] == '\n' || string[x] == '\t')
            string[x] = '\0';
        else
            return string;
    }
    return string;
}