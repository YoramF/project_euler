/*
https://projecteuler.net/problem=136

It is similar with problem 135 just that here we need to check up to 50M and for exatly one appearance.

Given the positive integers, x, y and z, are consecutive terms of an arithmetic progression.
These 3 integers can be presented as (y+d), y and (y-d).
So x^2-y^2-z^2=n ==> (y+d)^2 - y^2 - (y-d)^2 = n ==> 4*y*d-y^2 = n ==> y*(4*d-y) = n

In order to find n with exactly 1 possible solution we need to find n such that it has only one combinations of y & d such
that y*(4*d-y) = n.

Since n, x, y and z must be a positive integers, then 4*d >= y and y-d > 0.
Since we are looking for n < 50,000,000 it limits the ranges of y and d:
y = 1 .. 49,999,999.

Counts each result in array of size 50,000,000
Once done, scan the array and count the results with count = 1.

Solution:
2544559

real    0m1.478s
user    0m1.458s
sys     0m0.020s

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TARGET_NUM 50000000

int main () {
    int d, y;
    char *results;
    int i, c;
    long int n;

    if ((results = malloc(TARGET_NUM)) == NULL) {
        perror("Failed to allocate RAM");
        exit(EXIT_FAILURE);
    }

    memset(results, 0, TARGET_NUM);

    for (y = 1; y < TARGET_NUM; y++) {
        for (d = y/4+1; d < y; d++) {
            n = (long int)y*(long int)(4*d-y);        
            if (n < TARGET_NUM) {
                if (results[n] == 0)
                    results[n]++;
                else
                    results[n] = -1;
            }
            else
                // no point to check bigger values of d for given y - move to next y
                break;
        }
    }

    c = 0;
    for (i = 1; i < TARGET_NUM; i++)
        if (results[i] == 1) {
            c++;
        }


    printf("%d\n", c);
    free(results);

    return 0;
}