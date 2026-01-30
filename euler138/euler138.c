/*
https://projecteuler.net/problem=138


1) b & L are positive integers: b must be even
2) h = b+1 or b-1 ==> h must be a positive integer
3) since we are looking for b, h, L such that L^2 = (b/2)^2 + h^2 ==> 4L^2 = b^2 + 4h^2
For L to remain positive integer, b^2+4h^2 must be perfect square and it must be even.
Because this is a quadratic diophantine equation, the values of b follow a linear recurrence.
To find the "next" b, I can use the following jump constants.

For h = b+1
b_{n+1} = 9*b_n + 8*L_n + 8
L_{n+1} = 10*b_n + 9*L_n + 8

For h = b-1
b_{n+1} = 9*b_n + 8*L_n - 8
L_{n+1} = 10*b_n + 9*L_n - 8

we know b1 = 16, L1 = 17
using brute force approach I managed to iterate 6 times before long unsigned int overflows when using power of 2.
though I managed to find b2, b3, b4.. and found out that h1 = b1-1, h2 = b2+1, h3 = b3-1....

so all I need is to follow the rules above 12 times
sum L: 1118049290473932

real    0m0.005s
user    0m0.001s
sys     0m0.004s

*/

#include <stdio.h>

int main () {
    long unsigned int b, sumL = 0, L;
    long unsigned int b_n, L_n;
    int c = 1, add = -1;

    b_n = 16;
    L_n = 17;
    sumL = L_n;
    while (c < 12) {
        b = 9 * b_n + 8 * L_n + (add * 8);
        L = 10 * b_n + 9 * L_n + (add * 8);
        sumL += L;
        c++;
        b_n = b;
        L_n = L;
        add *= -1;
    }

    printf("sum L: %lu\n", sumL);

    return 0;
}