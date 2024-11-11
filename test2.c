#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
    int pid = getpid();
    printf(1, "When passed a value less than 1 or greater than 5\n");
    printf(1, "The system call changenice(int pid, int val)\n");
    printf(1, "will not change the nice value of the pid and will return -1.\n\n");
    printf(1, "PID = %d has nice value %d\n", pid, getnice(pid));
    printf(1, "Calling changenice(%d, -1)\n", pid);
    printf(1, "%d\n", changenice(pid, -1));
    printf(1, "PID %d has nice value %d\n", pid, getnice(pid));
    printf(1, "Calling changenice(%d, 6)\n", pid);
    printf(1, "%d\n", changenice(pid, 6));
    printf(1, "PID %d has nice value %d\n\n", pid, getnice(pid));
    printf(1, "Expected result is -1 after each call and the nice value remaining unchanged.\n");
    exit();
}