/*
https://projecteuler.net/problem=132

We can use problems 129 and 130 approach to find A(n) with a small changes:
1) No need to check for gdc() since we test only prime numbers
2) Need to check for valid results such that 1000000000 mod k == 0

Since both 2 and 5 can't divide repunit number and 3 devides repunits numbers with 3*n numbers of 1
We can start checking from 7 onward.

Found 40 Primes, Sum=843296

real    0m7.634s
user    0m7.605s
sys     0m0.014s
*/

#include <stdio.h>

#include <prime.h>

#define MAX_PRIMES 1000000
#define MAX_K   1000000000

/**
 * Calculate R(k)
 */
int A (int n) {
    int x, k;

    x = 1;
    k = 1;

    while (x != 0 && k <= MAX_K) {
        x = (x*10 + 1) % n;
        k++;
    }

    return x == 0? k: 0;
}

int main () {
    unsigned int *Primes, Ps;
    int c, i, a;
    unsigned int p, s;

    Primes = gen_prime(MAX_PRIMES, &Ps);

    c = 0;
    i = 4;
    s = 0;

    while (i < Ps && c < 40) {
        p = Primes[i];

        // check that A(p) is valid and that number of 1's devides 10^9
        if ((a = A(p)) > 0 && MAX_K % a == 0) {
            s += p;
            c++;
        }
        i++;
    }

    printf("Found %d Primes, Sum=%u\n", c, s);

}

