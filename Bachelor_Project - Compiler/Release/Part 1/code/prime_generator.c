#include "prime_generator.h"
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
