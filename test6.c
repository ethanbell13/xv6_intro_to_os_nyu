// Third test for priority scheduler
#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX 1000000000

int main()
{
    int pid = getpid(), nice = getnice(pid), i;
    printf(1, "PID %d has nice value %d\n", pid, nice);
    printf(1, "Calling fork()\n");
    if (fork() == 0)
    {
        changenice(getpid(), 1);
        if (fork() == 0)
        {
            changenice(getpid(), 1);
            for (i = 1; i <= MAX; i++)
                if (i % 100000000 == 0)
                    printf(1, "%d\n", i);
            exit();
        }
        for (i = 1; i <= MAX; i++)
            if (i % 100000000 == 0)
                printf(1, "%d\n", i);
        wait();
        exit();
    }
    // parent sleeps to ensure both fork calls go through
    sleep(20);
    printf(1, "\nPID %d has regained runtime\n", pid);
    wait();
    wait();
    exit();
}
