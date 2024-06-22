#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
extern char *current_direc;
extern char *undo;
extern char *original;
extern char *manjur;
extern char *to_change;
extern int flagger;
extern int ok;
extern int file_hai;
extern int directory_hai;
extern struct stat data;
extern int paster;
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "prompt.h"
#include "time.h"
#include "warp.h"
#include "trim.h"
#include "peek.h"
#include "seek.h"
#include "pastevents.h"
#include "neonate.h"
#include "iman.h"
#include "unistd.h"
#include "tokenize.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "proclore.h"
#include "signal.h"
#include "pwd.h"
#include "grp.h"
#include "time.h"
#include "signal.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "termios.h"

#define MAX 1000
extern int foregroundProcessPid;

struct ProcessInfo
{
    pid_t pid;
    int status;
    char **name;
    int sz;
    struct ProcessInfo *next;
};
// extern int itihas = 0;

struct ProcessInfo *removeProcess(struct ProcessInfo *head, pid_t pid);

struct node
{
    pid_t pid;
    int process_status;
    char proc_name[100];
    struct node *next;
};
typedef struct node node;
typedef struct myname
{
    char process_name[100];
    struct myname *next;
} myname;
extern char *command_pid[1000001];
extern char *s;
extern node *completedList;

#endif
