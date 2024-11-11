/*
Test Case 3: Priority Inheritance
Demonstrate the priority inheritance mechanism:
Show the original priority of the low-priority process
Show the priority elevation when inversion is detected
Show the priority restoration after lock release
Log all priority changes with timestamps (relative time is also fine)
*/

#include "types.h"
#include "stat.h"
#include "user.h"

#define KEEP_BUSY 1000000000
#define PRIORITY_2_CHILDREN 10

void interpretLockState(int resourceId);
int main()
{
    int pid = getpid(), resourceId = 0, i, nice, sum;
    interpretLockState(resourceId);
    changenice(pid, 5);
    nice = getnice(pid);
    printf(1, "PID %d has a nice value of %d\n", pid, nice);
    printf(1, "Calling lock(%d) in PID %d\n", resourceId, pid);
    lock(resourceId);
    printf(1, "Calling fork()\n\n");
    if (fork() == 0)
    {
        int childPid = getpid();
        changenice(childPid, 1);
        nice = getnice(childPid);
        printf(1, "PID %d has nice value %d\n", childPid, nice);
        printf(1, "Calling pilock(%d) in PID %d\n\n", resourceId, childPid);
        pilock(resourceId);
        printf(1, "PID %d has locked resource %d\n", childPid, resourceId);
        printf(1, "PID %d nice value has been returned to %d\n", pid, getnice(pid));
        resourcerelease(resourceId);
        exit();
    }
    else
    {
        // put parent to sleep so that child can print out all of its lines
        sleep(30);
        printf(1, "Spawning %d priority 2 processes that count to %d.\n", PRIORITY_2_CHILDREN, KEEP_BUSY);
        if (fork() == 0)
        {
            changenice(getpid(), 2);
            for (i = 0; i < PRIORITY_2_CHILDREN; i++)
                if (fork() == 0)
                {
                    for (i = 0; i < KEEP_BUSY; i++)
                        sum++;
                    exit();
                }
            for (i = 0; i < PRIORITY_2_CHILDREN; i++)
                wait();
            printf(1, "\nAll priority 2 children have terminated.\n");
            exit();
        }
        // put parent to sleep long enough for many priority 2 children to spawn
        sleep(10);
        printf(1, "PID %d nice value has changed from %d to %d\n", pid, nice, getnice(pid));
        resourcerelease(resourceId);
        wait();
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