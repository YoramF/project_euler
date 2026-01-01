/*
https://projecteuler.net/problem=131

The set of constraints we have for the expression N^3+N^2xP=M^3 :
1) N is a positive integer
2) P is a prime
3) As of the problem definition, P < 1,000,000
4) According to the problem definition N is unique per P
5) Except for N==1, all possible N's should be perfect cube  
6) Per P we can astimate the lower and upper values of N using the following:
    Upper value: P*sqrt(P)/3
7) For P2 > P2 N2 must be bigger than N1


Is is wnough to find N so that (1+P/N) is a perfect qube or (N+P) && N both are perfect qubes

Total: 173

real    0m4.835s
user    0m4.821s
sys     0m0.010s


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include <prime.h>

#define MAX_PRIMES  1000000


/**
 * Calculates the cube root using Newton's Method with integer arithmetic.
 * Returns the cube root if n is a perfect cube, otherwise returns 0.
 */
int perfect_cube_root_newton(int n) {
    if (n == 0) return 0;

    // Handle negative numbers
    int sign = (n > 0) ? 1 : -1;
    int target = (n > 0) ? n : -n;

    // Initial guess: the number itself is a safe (though slow) start.
    // A better guess would be 1 << (bits_in_target / 3).
    int x = target;
    int x_next;

    // Newton's Method Iteration
    while (1) {
        // formula: x_next = (2*x + target/(x*x)) / 3
        // x_next = (2 * x + target / (x * x)) / 3;
        x_next = (2 * x + (target / x) / x) / 3;

        // In integer math, the sequence might oscillate between two values 
        // near the root. If the guess stops changing or starts increasing, we've hit the limit.
        if (x_next >= x) {
            break;
        }
        x = x_next;
    }

    // Step 2: Verification
    // Check if x^3 is exactly equal to the target
    if (x * x * x == target) {
        return (int)(x * sign);
    }

    return 0; // Not a perfect cube
}

/**
 * Find upper bound for n given a prime number p
 * Use the following equation (k is the gap between N & M => k=M-N): P = 3*k + (3*k^2)/N + k^3/N^2
 * To simplify things we can use the following way: N(max)=P*sqrt(P)/3
 * 
 */
int upper_n (unsigned int p) {
    return p * (int)(sqrt((double)p) + 1) / 3;
}


int main () {
    int n, c, un, k, p, sk;
    unsigned int *Primes, Ps, ind;

    Primes = gen_prime(MAX_PRIMES, &Ps);

    ind = 0;
    c = 0;
    sk = 1;
    while (ind < Ps) {
        un = upper_n(Primes[ind]);
        k = sk;
        for (n = k*k*k; n < un; n = k*k*k) {
            p = perfect_cube_root_newton(n+Primes[ind]);
            if (p != 0) {
                    c++;
                  
                    // save k so that next loop will start with right N
                    sk = k;
                    break;
                }
                k++;
            }
        ind++;
    }

    printf("Total: %d\n", c);

    return 0;

}

