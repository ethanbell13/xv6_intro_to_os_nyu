/*
Test Case 1: Basic Lock/Unlock
Create a process that acquires and releases a lock
Verify the lock state before and after operations
Ensure proper handling of invalid lock IDs (0-7)
Create another process at the same priority and show how it waits to acquire a lock on a resourceId that hasn’t been released yet.
*/
#include "types.h"
#include "stat.h"
#include "user.h"

void interpretLockState(int resourceId);
int main()
{
    int pid = getpid(), resourceId = 0, i, nice = getnice(pid);
    interpretLockState(resourceId);
    printf(1, "Calling lock(%d)\n", resourceId);
    lock(resourceId);
    interpretLockState(resourceId);
    printf(1, "Calling resourcerelease(%d)\n", resourceId);
    resourcerelease(resourceId);
    interpretLockState(resourceId);
    printf(1, "\nCalling lock(-1)\n");
    lock(-1);
    printf(1, "Calling lock(8)\n");
    lock(8);
    printf(1, "Calling resourcerelease(-1)\n");
    resourcerelease(-1);
    printf(1, "Calling resourcerelease(8)\n");
    resourcerelease(8);
    printf(1, "\nPID %d has nice value %d\n", pid, nice);
    printf(1, "Calling lock(%d) in PID %d\n", resourceId, pid);
    lock(resourceId);
    printf(1, "Calling fork()\n\n");
    if (fork() == 0)
    {
        pid = getpid();
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
            printf(1, "Calling sleep(10) in PID %d\n", pid);
            sleep(10);
        }
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
