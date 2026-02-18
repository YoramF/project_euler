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

This version is using an async (private) library that allows me to run the computation function in up to 10 parallels.
We can see that the user time is almost identical, where real time is 10 times shorter.

Of course the idea with Euler project is to find the correct algorithm so that the overall time in single
thread will be minimal. Though using parallel is possible to find a fast solution is reasonable time and then work
on improving the algorithm.

sum: 878454337159

Sequantial running:
real    34m1.890s
user    34m1.649s
sys     0m0.002s

Parallel running:
real	3m35.935s
user	32m58.232s
sys	0m0.328s

*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include <async.h>


#define MAX_VAL 1000000000000
#define SEGMENT 10000

typedef struct {
    long unsigned int a, c;
} factors_t;

typedef struct {
    long unsigned int b;
    long unsigned int max_val;
} ex_func_arg_t;

typedef struct {
    long unsigned int sum;
    int count;
} ex_func_res_t;


/*
    find next factors of b. search start from lx.
    If found, update f and return true
*/
bool find_next_factors (long unsigned int b, long unsigned int sq, long unsigned int *lx, factors_t *f) {
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


void ex_func (void *arg, void *res) {
    ex_func_arg_t arg_v = *(ex_func_arg_t *)arg;
    ex_func_res_t *res_v = res;
    int count = 0;
    long unsigned int n, sum = 0L;
    long unsigned int nx, lsq, b2, b;
    factors_t f;

    b = arg_v.b;

    // printf("[ex_func] entered: b=%lu\n", b);

    while (b < arg_v.max_val) {

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
                    printf("[%lu:: %d, %lu] %lu: %lu:, %lu, %lu\n", arg_v.b, count, sum, n, f.a, b, f.c);
                    break;
                }
            }
        }

        b++;
    }

    // printf("[ex_func] exit: b=%lu, count=%d, sum=%lu\n", b, count, sum);

    res_v->count = count;
    res_v->sum = sum;
}


int main () {
    long unsigned int sum, sq;
    int count, i;
    ex_func_arg_t *ex_f_arg;
    ex_func_res_t *ex_f_res;
    long unsigned int segments = (MAX_VAL/SEGMENT);
    long unsigned int tb =  0;
    long unsigned int tb2 = 0;

    ex_f_res = malloc(segments*sizeof(*ex_f_res));
    ex_f_arg = malloc(segments*sizeof(*ex_f_arg));
    
    sum = 9;
    count = 1;
    if (async_init(10) < 0) {
        printf("Failed to set async env\n");
        exit(EXIT_FAILURE);
    }

    sq = (long unsigned int)sqrt(MAX_VAL);

    i = 0;
    tb = 2;
    tb2 = SEGMENT;
    while (i < segments && tb < sq) {
        ex_f_arg[i].b = tb;
        ex_f_arg[i].max_val = tb2;
        if (async_launch(ex_func, NULL, &ex_f_arg[i], &ex_f_res[i], NULL) < 0) {
            printf("Failed to loanch function\n");
            exit(EXIT_FAILURE);
        }
        
        i++;
        tb = i * SEGMENT;
        tb2 = tb + SEGMENT;
    }

    async_terminate();

    for (i = 0; i < (MAX_VAL/SEGMENT); i++) {
        sum += ex_f_res[i].sum;
        count += ex_f_res[i].count;
    }
  
    free(ex_f_arg);
    free(ex_f_res);

    printf("count: %u, sum: %lu\n", count, sum);

    return 0;

}