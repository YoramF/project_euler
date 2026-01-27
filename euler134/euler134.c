/**
 * https://projecteuler.net/problem=134
 * 
 * First try will be a brute force approach.
 * Go through all pair of primes where 5 <= p1 <= 1,000,000 and p2 is the consecutive prime to p1.
 * Start with n = m * p2 (m = 3, 5, ... ) until is_last_digits(n,p1) == true.
 * is_last_digits(n,p1) is a function that retrieve true last digits of n are equal to p1.
 * (m) must be an odd number since n last digit can't be 0, 2, 4, 6 or 8, due to the fact the p1 is a prime.
 * 
 * Sum: 18613426663617118

 * real    1m13.226s
 * user    1m13.209s
 * sys     0m0.006s
 */

 #include <stdio.h>
 #include <stdbool.h>

 #include <prime.h>

 #define MAX_PRIME 1000200

 bool is_last_digitd (long unsigned int n, unsigned int p) {
    int dn, dp;

    while (n && p) {
        dn = n % 10L;
        dp = p % 10;
        if (dn != dp)
            return false;

        n /= 10L;
        p /= 10;
    }

    if (p == 0)
        return true;

    return false;
 }


 int main () {
    unsigned int *primes, ps;
    unsigned int p1, p2;
    unsigned int i, m;
    long unsigned int n, sum = 0L;

    primes = gen_prime(MAX_PRIME, &ps);

    for (i = 2; primes[i] <= 1000000; i++) {
        p1 = primes[i];
        p2 = primes[i+1];

        // m must be an odd number
        m = 3;
        n = m * (long unsigned int)p2;

        while (!is_last_digitd(n, p1)) {
            m += 2;     // keep m odd.
            n = (long unsigned int)m * (long unsigned int)p2;
        }
        sum += n;
    }

    free(primes);
    printf("Sum: %lu\n", sum);

    return (0);

 }