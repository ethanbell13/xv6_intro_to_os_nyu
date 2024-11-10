#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
    int parentPid = getpid(), childPid, nice = getnice(parentPid);
    if ((childPid = fork()) == 0)
    {
        printf(1, "parentPid = %d\n", parentPid);
        printf(1, "parent nice value = %d\n", nice);
        changenice(parentPid, 1);
        nice = getnice(parentPid);
        printf(1, "After nice(%d,1), getnice(%d) = %d\n", parentPid, parentPid, nice);
        sleep(.1);
        exit();
    }
    else
    {
        nice = getnice(childPid);
        printf(1, "Child pid = %d\ngetnice(%d) = %d\n", childPid, childPid, nice);
        changenice(childPid, 1);
        nice = getnice(childPid);
        printf(1, "After nice(%d,1), getnice(%d) = %d\n", childPid, childPid, nice);
        wait();
    }
    exit();
}