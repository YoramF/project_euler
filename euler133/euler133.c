/*
https://projecteuler.net/problem=133

Solution: Go over all Primes below 100,000. Ignore prime==2 and add it to the sum since
there is no R(n) that is divisible by 2.
Calculate R(n) for all these Primes. Take the result and see if it devides 10^n. If not, add that
Prime to the sum.

sum_p: 453647705

real    0m3.037s
user    0m3.034s
sys     0m0.003s
*/

#include <stdio.h>

#include <prime.h>


/**
 * Check that n is divisdes 10^n
 * Facts:
 * 1) 2^n divides 10^n
 * 2) 5^n devides 10^n
 */
int d_10n (int n) {
    int t;

    if (n > 0) {
        t = n;

        while (t % 5 == 0) {
            t /= 5;
        }
       
        if (t == 1)
            return 1;

        while (t % 2 == 0)
            t /= 2;

        if (t == 1)
            return 1;

        return 0;
    }

    return 0;
}

/**
 * Calculate R(k). Since n is only prime number, A() must return an integer value
 */

int A (int n) {
    int x, k;

    x = 1;
    k = 1;

    while (x != 0) {
        x = (x*10 + 1) % n;
        k++;
    }

    return k;
}


int main () {
    int an, a, n;
    unsigned int *primes, ps, p;
    unsigned int sum_p;

    primes = gen_prime(100000, &ps);

    n = 3;              // start with prime == 7
    sum_p = 2+3+5;      // first 3 primes that can't be factor of R(10^n)

    while (n < ps) {
        p = primes[n];
        a = A(p);
        an = d_10n(a);
        if (an == 0)
            sum_p += p;
        n++;
    }

    printf("sum_p: %u\n", sum_p);

    return 0;
}