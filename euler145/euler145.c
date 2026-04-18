/**
 * https://projecteuler.net/problem=145
 * 
 * Brute Force
 */

#include <stdio.h>
#include <stdbool.h>

#define LIMIT 1000000000

bool is_odd_digits (int n) {
    int d;

    while (n) {
        d = n % 10;
        if (d % 2 == 0)
            return false;
        n /= 10;
    }

    return true;
}

/**
 * if the reversed number has will have leading zerros return -1, otherwize return the revesed number
 */
int reverse_number (int n) {
    int r_n, d;

    d = n % 10;
    if (d == 0)
        return -1;

    r_n = 0;
    while (n) {
        d = n % 10;
        r_n = r_n * 10 + d;
        n /= 10;
    }

    return r_n;
}


int main () {
    int n, sum, r_n, count;

    count = 0;
    for (n = 12; n < LIMIT; n++) {
        if ((r_n = reverse_number(n)) < 0)
            continue;
        sum = n + r_n;
        if (is_odd_digits(sum)) {
            printf("n: %d, r_n: %d, sun: %d\n",n, r_n, sum);
            count++;
        }

    }

    printf("count: %d\n", count);
}