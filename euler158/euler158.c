/**
 * https://projecteuler.net/problem=158
 * 
 * Solution approach:
 * We assume we have a list if 26 chars orders in descanding order from left to right.
 * 1) For a given n (2 <= n <= 26) there are C(26,n) combinations where C(26,n) = 26!/(n!*(26-n)!).
 * 2) Then we need to count how many way we can order k (1 <= k < n) chars so that single char will have a lower value
 * char as its left neighbour.
 * A string of these n distinct chars has exactly one char that comes lexicographically after its neighbor to
 * the left if and only if it splits into two decreasing substrings (one left part and one right part) meeting at that
 * single increasing transition, which gives (2^n - n - 1) valid arrangements for those specific n chars
 * 
 * Therefore the solution for p(n) is multiply (1) by (2)
 * 
 * (2)** I needed google help to get to this solution of two decreasing substrings and how to count how 
 * many of them are there for a given n
 * 
 */

 #include <stdio.h>

 #define MAX_CHARS  26

// calculate binomial formula. limited to n<=30
int ncm (int n, int m) {
    int r, i;

    r = 1;
    for (i = 1; i <= m; i++) {
        r *= (n - i + 1);
        r /= i;
    }

    return r;
}

// return n^p
long int power (int n, int p) {
    long int r = 1;

    while (p) {
        r *= n;
        p--;
    }

    return r;
}

// calculate p(n)
long int pn (int n) {
    int g;

    // first calculate how many groups of n chars we can have out of MAX_CHARS chars
    g = ncm(MAX_CHARS, n);

    return (long int)g * (power(2, n) - (long int)n - 1L);
}

int main () {
    int i;
    long int mp = -1, p;

    for (i = 2; i <= MAX_CHARS; i++) {
        p = pn(i);
        if (p > mp)
            mp = p;
        printf("Pn(%d) = %ld\n", i, p);
    }

    printf("%ld\n", mp);
    return 0;
}