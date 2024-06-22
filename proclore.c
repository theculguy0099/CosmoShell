#include "proclore.h"
/*********************************************************

The "proclore" function gathers and displays process details from Linux's "/proc" filesystem.
It uses a given process ID to determine status, group, and memory usage.
It accesses executable paths via symbolic links.
The code includes error handling for file access and reading issues,
 providing comprehensive process insight.

****************************************************************/
void proclore_function(pid_t pid)
{
    printf("pid: %d\n", pid);
    pid_t process_g_id = getpgid(pid);
    char running_status;
    char info_path[50];
    sprintf(info_path, "/proc/%d/stat", pid);
    FILE *info_file = fopen(info_path, "r");
    if (!(info_file != NULL))
    {
        perror("Error in stat file opening");
    }
    else if (info_file != NULL)
    {
        fscanf(info_file, "%*d %*s %c", &running_status);
        fclose(info_file);
        if (running_status == 'w')
            running_status = 'Z';

        printf("process Status: %c", running_status);
        if (pid == process_g_id)
        {
            printf("+");    
        }
        printf("\n");
    }
    printf("Process Group: %d\n", process_g_id);
    char memory_path[50];
    sprintf(memory_path, "/proc/%d/status", pid);
    FILE *virtual_file = fopen(memory_path, "r");
    if (virtual_file == NULL)
    {
        perror("Error opening status file");
    }
    else
    {
        char scan[256];
        while (fgets(scan, sizeof(scan), virtual_file))
        {
            if (strstr(scan, "VmSize:") != NULL)
            {
                printf("Virtual Memory: %s", scan);
                break;
            }
        }
        fclose(virtual_file);
    }
    char executable[256];
    sprintf(executable, "/proc/%d/exe", pid);
    char handled_path[MAX];
    ssize_t length = readlink(executable, handled_path, sizeof(handled_path) - 1);
    if (length == -1)
    {
        perror("Error reading executable path");
    }
    else
    {
        handled_path[length] = '\0';
        printf("executable Path: %s\n", handled_path);
    }
}
