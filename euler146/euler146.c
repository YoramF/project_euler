/*
https://projecteuler.net/problem=146

To find the set of integers that satisfy the problem definition is by testing all integers up to sqrt(150M).
Each number n will be tested if n^2+1, n^2+3, n^+7, n^2+9, n^2+13 and n^2+27 are consecutive primes.

If it does, add it to the general count.

Consideerations to take:
1) for n^2+1 to be prime, for n > 2, n must be even.
2) since n must be even, in order to meat the problem requrement the last digit of n must be 0, otherewise n^2+1 will
be even and therefore not a prime. So n must be a multiply of 10's

*/

#include <stdio.h>
#include <stdbool.h>

#include <prime.h>

#define MAX_VAL 150000000

/**
 * find next prime number
 */
long unsigned int n_prime (long unsigned int p) {

    do {
        p += 2;
    } while (!is_prime(p));

    return p;
}

/**
 * Fast check if a given n^2+1, n^2+3, n^2+7, n^2+9, n^2+13 and n^2+27 are all prime numbers
 */
bool fast_prime_check (long unsigned int n2)
{
    long unsigned int i;

      if((n2 + 1) % 2 == 0)
            return false;

      for (i = 3; i * i <= (n2 + 27); i += 2) {
            if((n2 +1 ) % i == 0)
                  return false;

            if((n2 + 3) % i == 0)
                  return false;
                  
            if((n2 + 7) %i == 0)
                    return false;

            if((n2 + 9) % i== 0)
                    return false;
                    
            if((n2 + 13) %i ==0)
                    return false;

            if((n2 + 27) % i == 0)
                    return false;
      }

      return true;
}


/**
 * Return true if n match the problem request
 */
bool is_consec (unsigned int n) {
    long unsigned int np, n2, ln = (long unsigned int)n;

    n2 = ln * ln;

    // first check if n^2+1, n^2+3, n^2+7, n^2+9, n^2+13 and n^2+27 are all prime numbers
    if (!fast_prime_check(n2))
        return false;

    // now check that n^2+1, n^2+3, n^2+7, n^2+9, n^2+13 and n^2+27 are consecutive primes
    np = n2 + 1;
    if (!is_prime(np))
        return false;

    np = n_prime(np);
    if (np != (n2 + 3))
        return false;

    np = n_prime(np);
    if (np != (n2 + 7))
        return false;

    np = n_prime(np);
    if (np != (n2 + 9))
        return false;

    np = n_prime(np);
    if (np != (n2 + 13))
        return false;

    np = n_prime(np);
    if (np != (n2 + 27))
        return false;

    return true;
}

int main () {
    long unsigned int sum;
    unsigned int n;

    sum = 0L;

    for (n = 10; n < MAX_VAL; n += 10) {
        if (is_consec(n)) {
            sum += (long unsigned int)n;
            printf("n: %u, sum: %lu\n", n, sum);
        }
    }

    printf("sum: %lu\n", sum);

    return 0;
}
