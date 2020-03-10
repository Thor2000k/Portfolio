#include "misc.h"
#include <math.h>

int prime_next(int lower_bound)
{
    if (lower_bound % 2 == 0)
    {
        lower_bound++;
    }

    while (!prime_check(lower_bound))
    {
        lower_bound++;
    }

    return lower_bound;
}

int prime_check(int num)
{
    if (num == 2)
    {
        return 1;
    }
    else if (num % 2 == 0) 
    {
        return 0;
    }
    else
    {
        int sq = sqrt(num);
        for (int divisor = 3; divisor <= sq; divisor += 2)
        {
            if (num % divisor == 0)
            {
                return 0;
            }
        }
    }
    
    return 1;
}

int get_flag(int flags, int pos)
{
    return (flags >> pos) & 1;
}

void set_flag(int *flags, int pos)
{
    if (!get_flag(*flags, pos))
    {
        *flags += 1 << pos;
    }
}
