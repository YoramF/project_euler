/*
https://projecteuler.net/problem=141

Starting with brute force approach of iterating through n < 10^7 and n is perfect square and folowing these rules:
0 < a < b < c,
a = n % b;
c = n / c;
b/a = c/b

we got the following list:

n=9 a=1, b=2, c=4
n=10404, a=36, b=72, c=144
n=16900: a=25, b=75, c=225
n=7344, a=8, b=92, c=1058
n=576081, a=81, b=360, c=1600
n=6230016, a=512, b=1472, c=4232
n=7322436, a=1936, b=2420, c=3025

Since we know a < b and a must be a divisor of b^2 (to ensure c = b^2/a is an integer), the most reliable way to calculate n
based on b alone is to iterate through the divisors of b^2.
Here is the step-by-step logic:
List Divisors: Find all integers a that are divisors of b^2 such that a < b.
Calculate c: For each a, calculate c = b^2 / a.
Check Order: Ensure b < c (this is automatically true if a < b).
Calculate n: Use the formula n = bc + a.
Verify Square: Check if sqrt(n) is an integer.

sum: 878454337159

real    34m1.890s
user    34m1.649s
sys     0m0.002s

*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define MAX_VAL 1000000000000

typedef struct {
    long unsigned int a, c;
} FACTORS;


/*
    find next factors of b. search start from lx.
    If found, update f and return true
*/
bool find_next_factors (long unsigned int b, long unsigned int sq, long unsigned int *lx, FACTORS *f) {
    long unsigned int x, y;

    for (x = *lx; x < sq; x++) {
        if (b % x == 0) {            
            y = b / x;

            // y must be bigger than x
            if (y <= x) 
                return false;
            f->a = x;
            f->c = y;
            *lx = x+1;
            return true;
        }
    }
    return false;
}

int main () {
    long unsigned int n, sum;
    long unsigned int b, nx, sq, lsq, b2;
    int count;
    FACTORS f;

    sum = 9;
    b = 4;
    n = 0;
    count = 1;
    b2 = 16;

    sq = (long unsigned int)sqrt(MAX_VAL);

    while (b < sq) {

        nx = 2;
        b2 = b*b;
        while (find_next_factors(b2, b, &nx, &f)) {
            n = b * f.c + f.a;
            if (n < MAX_VAL) {
                // check if nt is perfect square
                lsq = (long unsigned int)sqrt(n);
                if (lsq*lsq == n) {
                    sum += n;
                    count++;
                    printf("[%d, %lu] %lu: %lu:, %lu, %lu\n", count, sum, n, f.a, b, f.c);
                    break;
                }
            }
        }

        b++;
    }

    printf("count: %u, sum: %lu\n", count, sum);


    return 0;

}