/*
https://projecteuler.net/problem=135

Given the positive integers, x, y and z, are consecutive terms of an arithmetic progression.
These 3 integers can be presented as (y+d), y and (y-d).
So x^2-y^2-z^2=n ==> (y+d)^2 - y^2 - (y-d)^2 = n ==> 4*y*d-y^2 = n ==> y*(4*d-y) = n

In order to find n with exactly 10 possible solutions we need to find n such that it has 10 combinations of y & d such
that y*(4*d-y) = n.

Since n, x, y and z must be a positive integers, then 4*d >= y and y-d > 0.
Since we are looking for n < 1,000,000 it limits the ranges of y and d:
y = 1 .. 999,999.

Counts each result in array of size 1,000,000
Once done, scan the array and count the results with count = 10.

Solution:
4989

real    0m0.008s
user    0m0.005s
sys     0m0.002s

*/

#include <stdio.h>

int main () {
    int d, y;
    int results [1000000] = {};
    int i, c;
    long int n;

    for (y = 1; y < 1000000; y++) {
        for (d = y/4+1; d < y; d++) {
            n = (long int)y*(long int)(4*d-y);
            if (n < 1000000L)
                results[n]++;
            else
                // no point to check bigger values of d for given y - move to next y
                break;
        }
    }

    c = 0;
    for (i = 1; i < 1000000; i++)
        if (results[i] == 10) {
            c++;
        }


    printf("%d\n", c);

    return 0;
}