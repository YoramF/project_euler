/*
https://projecteuler.net/problem=130

Based on problem 129, we just need to look for non-primes n's

Sum: 149253

real    0m0.158s
user    0m0.149s
sys     0m0.008s

*/

#include <stdio.h>

#include <prime.h>

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

int A (int n) {
    int x, k;

    if (gdc(n,10) == 1) {
        x = 1;
        k = 1;

        while (x != 0) {
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
    int n, k, c, d, s;

    c = 0;
    n = 91;
    s = 0;

    while (c < 25) {
        if (!is_prime(n)) {
            if ((k = A(n)) > 0) {
                d = (n-1) % k;
                if (d == 0) {
                    c++;
                    s += n;
                    // printf("[%d] %d\n", c, n);    
                }       
            }
        }
        n++;
    }

    printf("Sum: %d\n", s);

    return 0;
}