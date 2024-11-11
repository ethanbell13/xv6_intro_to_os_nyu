#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
    int pid = getpid(), childPid;
    printf(1, "Both of the changenice() and getnice() system calls return -1 if the pid isn't found.\n");
    printf(1, "Calling childPid = fork()\n");
    if ((childPid = fork()) == 0)
        exit();
    else
    {
        printf(1, "Calling wait() in parent\n");
        wait();
        printf(1, "Child process has terminated\n");
        printf(1, "childPid was %d\n", childPid);
        printf(1, "Calling changenice(%d, 1)\n", childPid);
        printf(1, "%d\n", changenice(childPid, 1));
        printf(1, "Calling getnice(%d, 1)\n", childPid);
        printf(1, "%d\n", getnice(childPid));
        printf(1, "\nBoth system calls should have output -1\n\n");
        printf(1, "changenice should return prior nice value\n");
        printf(1, "getnice(%d) = %d\n", pid, getnice(pid));
        printf(1, "changenice(%d, 1) returns %d\n", pid, changenice(pid, 1));
    }
    exit();
}