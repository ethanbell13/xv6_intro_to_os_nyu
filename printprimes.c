#include "types.h"
#include "stat.h"
#include "user.h"

unsigned int PRIME_MAX = 1009;

int sqrt(int x);

int main()
{
    unsigned int i, j, prime;
    for (i = 2; i < PRIME_MAX; i++)
    {
        prime = 1;
        for (j = 2; j <= sqrt(i); j++)
            if (i % j == 0)
            {
                prime = 0;
                break;
            }
        if (prime)
            printf(1, "%d\n", i);
    }
    exit();
}

int sqrt(int x)
{
    int start = 1, end = x, result;
    if (x < 0)
        return -1;
    if (x < 2)
        return x;
    while (start <= end)
    {
        int mid = (start + end) / 2;
        // x / mid is used to account for integer value that is close to a float sqrt
        if (mid == x / mid)
            return mid;
        if (mid < x / mid)
        {
            start = mid + 1;
            result = mid;
        }
        else
        {
            end = mid - 1;
        }
    }
    return result;
}