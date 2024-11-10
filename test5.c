// new test, prints out process execution sequence and process table
// currently it works fine, if it has 3 processes and a smaller number for LOOPS
// it just finishes
// but not yet showing the effect of priority scheduling
// now testing numbers and structures to get what I want to show

//  Second test for priority scheduler
#include "types.h"
#include "stat.h"
#include "user.h"

#define LOOPS 400000000
#define PCOUNT 5

int main()
{
    int i, pid = getpid();

    for (i = PCOUNT; i >= 1; i--)
    {
        if (fork() == 0)
        {
            // print table to see current status
            printtable();

            // demonstrate in child process, show initial pid and nice value
            pid = getpid();
            int oldnice = getnice(pid);
            printf(1, "pid : %d\toldpriority : %d\n", pid, oldnice);
            changenice(pid, i);

            // print table to see current status
            printtable();

            // printing just prints a mess, so printing will be done in scheduler instead
            // useless code used to occupy cpu
            int x, z;
            for (z = 0; z < LOOPS; z++)
                x = x + 3.14 * 89.64;

            oldnice = getnice(pid);
            printf(1, "pid : %d\toldpriority : %d\n", pid, oldnice);
            changenice(pid, i);

            // print table to see current status
            printtable();

            for (z = 0; z < LOOPS; z++)
                x = x + 3.14 * 89.64;

            printf(1, "pid : %d\tcomplete\n", pid);

            exit();
        }
    }
    for (i = 0; i < PCOUNT; i++)
        wait();
    exit();
}