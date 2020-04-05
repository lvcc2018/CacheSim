#ifndef UTILS
#define UTILS


unsigned long long log_two(unsigned long long natural)
{
    unsigned long long count = 0;
    unsigned long long sum = 1;
    while (1)
    {
        sum <<= 1;
        count += 1;
        if (sum >= natural)
        {
            return count;
        }
    }
}

#endif