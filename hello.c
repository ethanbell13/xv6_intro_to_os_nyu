#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    if (argc > 1)
        printf(2, "hello should only have one argument.\n");
    else
        printf(1, "Hello, World!\n");
    exit();
}