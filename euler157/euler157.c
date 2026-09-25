/**
 * https://projecteuler.net/problem=157
 * 
 * To solve 1/a+1/b=p/n we can do the same arithmetics as we did in problem 108.
 * 
 * Step 1: 
 * Algebraic Transformation: Start by multiplying through by a*b*n: n*(a + b) = c*a*b 
 * Rearrange into a single expression set to zero: c*a*b - n*a - n*b = 0
 * To apply Simon's Favorite Factoring Trick (just as in the p=1 case), multiply the entire equation by c:
 *  p^2*a*b - p*n*a - c*n*b = 0
 * Now add n^2 to both sides: p^2*a*b - p*n*a - p*n*b + n^2 = n^2
 * Factor the left side by grouping: (p*a - n)(p*b - n) = n^2
 * 
 * Step 2:
 *  The Direct Substitution AnalogIf we set: a' = p*a - n and b' = p*b - n
 *  we get the classic relationship: a'*b' = n^2
 *  To express a and b explicitly in terms of these factors: a = (n + a')/p,  b = (n + b')/p 
 * 
 * Step 3:
 * Finding the Valid Integer SolutionsFor p = 1, every pair of factors $(a', b') of n^2 produces valid integer
 * solutions for a and b. For p > 1, finding all integer solutions (a, b) proceeds by:
 * Find all pairs of integer factors (a', b') of n^2 such that a'*b' = n^2.
 * Filter by the congruence condition:
 * Keep only those pairs where: a' % p = -n % p  and b' % p = -n % p (This guarantees that n + a' and n + b' are both
 * divisible by p, ensuring a and b are integers.
 * Compute a and b: a = (n + a')/p, b = (n + b')/p
 * 
 * Step4:
 * We need to count only cases where a < b
 * 
 * Now to find the solution for the problem itself, we need to check how many solutions we have to all 10^n where
 * 1 <= n <= 9, We need to count only those where a <= b.
 * I will use the gen_factorials() from problem 110 and stop once we generate a 3 factors set, since we need to count
 * only cases with 2 factors.
 * Once we have the list of 2 factors, we need to filter those which are not comply with
 * a' % p = -n % p  and b' % p = -n % p
 * 
 * This approach is an optimized brute force.
 * Since we are actually looking for a solution of the form 1/a + 1/b = p/(10^n), we need to set the limit for
 * p whihc is 2 * 10^n --> largest 1/a and 1/b is 1/1, therefore: 1/1 + 1/1 = (2 * (10^n))/(10^n)
 * 
 * For 10^18 there are 181 factors pairs. Thes pairs will be used to generate all possibe 1/a+q/b=p/10^n. Therefore there
 * is no need to generate all these pairs again per each value of p. Even with this optimization, the time for computing
 * solutions for 10^8 and 10^9 is quite long.
 * 
 * Solution
 * sum: 53490
 * 
 * real    28m11.972s
 * user    28m11.810s
 * sys     0m0.000s
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define MAX_VALID_P 200                     // it is actually 181

typedef struct {
    long int a, b;
} pair_t;

long unsigned power (unsigned b, int p) {
    long unsigned r = 1L;

    while (p) {
        r *= b;
        p--;
    }

    return r;
}


int count_factors (long int n, long int p, pair_t *pairs) {
    long int a, b, n2;
    long int nmp;
    int r, ind;

    r = 0;
    nmp = (-n) % p; 
    if (nmp < 0)
        nmp += p;

    // check if this is the first check for given n
    if (p == 1L) {
        ind = 0;
        n2 = n*n;
        for (a = 1; a*a <= n2; a++) {
            b = n2 / a;
            if (b * a == n2) {
                pairs[ind].a = a;
                pairs[ind].b = b;
                ind++;
                r++;
            }
        }
        // mark end of list
        pairs[ind].a = n2;
    }
    else {
        // now we just need to check the precomputed factor pairs
        ind = 0;
        a = pairs[ind].a;
        while (a <= n) {
            b = pairs[ind].b;

            // check a' % p = -n % p  and b' % p = -n % p
            if ((a % p == nmp) && (b % p) == nmp)
                r++;

            ind++;
            a = pairs[ind].a;
        }
    }

    return r;
}

int main (int argc, char **argv) {
    long unsigned int num, p;
    long unsigned int sum = 0L;
    int i, f;
    pair_t *pairs;

    if ((pairs = malloc(MAX_VALID_P * sizeof(*pairs))) == NULL) {
        perror("Failed to allocate RAM");
        exit(EXIT_FAILURE);
    }

    for (i = 1; i <= 9; i++) {
        num = power(10, i);

        // generate the list of valid pairs for p
        f = count_factors(num, 1L, pairs);
        sum += f;

        // continue with the rest of p
        for (p = 2; p <= 2*num; p++) {
            f = count_factors(num, p, pairs);

        sum += (long int)f;
        }
    }

    free(pairs);
    printf("sum: %lu\n", sum);
    return 0;

}
