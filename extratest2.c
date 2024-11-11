#include "types.h"
#include "stat.h"
#include "user.h"

void interpretLockState(int resourceId);
int main()
{
    int pid = getpid(), resourceId = 0, i, nice;
    interpretLockState(resourceId);
    changenice(pid, 5);
    nice = getnice(pid);
    printf(1, "PID %d has a nice value of %d\n", pid, nice);
    printf(1, "Calling lock(%d) in PID %d\n", resourceId, pid);
    lock(resourceId);
    printf(1, "Calling fork()\n\n");
    if (fork() == 0)
    {
        pid = getpid();
        changenice(pid, 1);
        nice = getnice(pid);
        printf(1, "PID %d has nice value %d\n", pid, nice);
        printf(1, "Calling lock(%d) in PID %d\n\n", resourceId, pid);
        lock(resourceId);
        printf(1, "PID %d has locked resource %d\n", pid, resourceId);
        resourcerelease(resourceId);
        exit();
    }
    else
    {
        // put parent to sleep so that child can print out all of its lines
        sleep(30);
        nice = getnice(pid);
        for (i = 0; i < 5; i++)
        {
            printf(1, "PID %d with nice value %d has been allocated runtime and still holds resource %d\n", pid, nice, resourceId);
            printf(1, "Calling sleep(20) in PID %d\n", pid);
            sleep(20);
        }
        printf(1, "The high priority process has slept through at least 5 rutime allocations while waiting for the low priority process to release the lock\n");
        printf(1, "Calling resourcerelase(%d) in PID %d\n", resourceId, pid);
        resourcerelease(resourceId);
        wait();
    }
    exit();
}

void interpretLockState(int resourceId)
{
    int result;
    if ((result = lockstate(resourceId)) == -1)
        printf(1, "Error in lockstate system call!\n");
    else if (result == 1)
        printf(1, "resource %d is locked\n", resourceId);
    else
        printf(1, "resource %d is unlocked\n", resourceId);
}