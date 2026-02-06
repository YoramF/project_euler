/*
https://projecteuler.net/problem=139

Let a & b be the legs (b the long one) of the right triangle and c is the Hypotenuse.
We need to find right triangles so that a+b+c < 100,000,000.
b > a otherewise no middle square exists.
c % (b - a) == 0; Let K (the parimeter) = a + b + c
The above resteriction brings us to "Primitive Triangle", were b-a is always 1.

We need to find set of "Primitive triangle" that comply with c % (b-a) == 0. In fact it force b-a to always be 1.
Once we find the set of these triangle we can generate from each promitive triangle set of other triangles of the
form n * (a, b, c) that their parimeter is less than 100,000,000

Solution:
count: 10057761

real    0m0.141s
user    0m0.134s
sys     0m0.007s

*/
#include <stdio.h>
#include <math.h>

#define MAX_PARIMETER 100000000

int main () {
    long unsigned int a2, b2, c2;
    unsigned int a, b, c, n;
    unsigned count;
    unsigned parimeter;

    count = 0;
    parimeter = 3 + 4 + 5;
    a = 2;
    while (true) {
        a++;
        b = a + 1;
        a2 = (long unsigned int)a*a;
        b2 = (long unsigned int)b*b;
        c2 = b2+a2;
        c = (unsigned int)sqrt((double)c2);
        parimeter = a+b+c;
        if (parimeter < MAX_PARIMETER) {
            // check if we found a "Primitive Triangle" with parimeter in range
            if ((unsigned long int)c*c == c2)
                // we found "Primitive Triangle".
                // Any multiplication of this triangle is a valid one
                // Just count how many variation of this triangle have parimeter in range
                count += MAX_PARIMETER / parimeter;
        }
        else
            break;
    }

    printf("count: %u\n", count);
}