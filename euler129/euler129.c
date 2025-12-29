/*
https://projecteuler.net/problem=129

Based on the following Repunit Divisibility rule:
Base & Coprime Numbers:
    If a number n and the base b are coprime (share no common factors, e.g., n=7, b=10),
    then there's always a repunit R(k) in base b divisible by n.
The fact that we are looking for an n with R(k) is divisible by n, we can use long division algorithm 
to find k without using the actual huge numbers.

Solution:
The least value of n for which A(n) first exceeds one-million is 1000023,  A(1000023)=1000020

real    0m0.026s
user    0m0.026s
sys     0m0.000s

*/

#include <stdio.h>

int gdc (int a, int b) {
    int x;

    x = a % b;
    while (x != 0) {
        a = b;
        b = x;
        x = a % b;
    }

    return b;
}

// calculate number of 1's in a repunit number that is divisible by n
int A (int n) {
    int x, k;

    if (gdc(n,10) == 1) {
        x = 1;
        k = 1;

        while (x != 0) {
            // x is a repunit number
            x = (x*10 + 1) % n;
            k++;
        }

        return k;
    }
    else {
        return 0;
    }
}

int main () {
    int n, k;

    // n is always greater than k
    n = 1000002;
    
   while (1) {
        k = A(n);
        if (k > 1000000)
            break;
        n++;
    }

    printf("The least value of n for which A(n) first exceeds one-million is %d,  A(%d)=%d\n", n, n, k);

    return 0;
}