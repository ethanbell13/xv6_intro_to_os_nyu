#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    // printf(1, "In strace.c\n");

    // only valid uses : strace on / strace off
    // check if there are 2 arguments
    // otherwise print error
    if (argc < 2)
    {
        printf(2, "Usage : strace on, strace off, or strace<command>\n");
        exit();
    }
    int on = strcmp(argv[1], "on"); // equal = 0!
    int off = strcmp(argv[1], "off");

    if (on == 0)
        straceon();
    else if (off == 0)
        straceoff();
    else
    {
        if (fork() == 0)
        {
            char **newArgv = &(argv[1]);
            set_proc_strace();
            exec(argv[1], newArgv);
            printf(1, "Exec failed for strace<command>\n");
        }
        wait();
    }
    exit();
}