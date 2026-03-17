/**
 * https://projecteuler.net/problem=143
 * 
 * There are few related formulas I can use:
 * Calculating Torricelli number L:
 *  
 * solution option:
 * Iterate p, q, r and generate a triangles ABC where tgheir sides a, b and c are calculated:
 * c^2 = a^b + b^2 - 2*a*b*cos(120) = a^2+b^+a*b
 * This equation of (a,b,c) is called Eisenstein triples. They can be calculated using the following:
 *  a = m*m-n*n;
 *  b = 2*n*m+n*n;
 *  c = m*m+n*n+n*m;
 * Were m > n > 0 and these equations guaranty the a, b & c are positive integers.
 * If we want to prove that 3 vectors (p, q, r) from 3 vertexs of a triangle are crossing at Torricelli/Fermant
 * point we need to show that they are crossing each other with 120 degree angle and that the opposite triangle side
 * is positive integer.
 * The above formulas will be used to find p & q and to find r we will iterate from 1 up untill we find positive 
 * integers A, B such that C^2=p^2+r^2+p*r and A^2=q^2+r^2+q*r
 * Once we find the triple (p,q,r) we sum then and check that theh sum is less than 120,000
 * 

 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VAL 120000

typedef struct {
    int a, b, c;
} triple_t;



/**
 * Given (p,q) return r such that p,q &r are the 3 distances to Torricelli/Fermant point.
 * If we can't find such r return 0;
 */
int find_r (int p, int q) {
    long unsigned int a2, b2, a, b, lp, lq, lr;
    int r, max;

    max = MAX_VAL-p-q;
    lp = (long unsigned int)p;
    lq = (long unsigned int)q;

    for (r = 1; r <= max; r++) {
        lr = (long unsigned int)r;

        a2 = (lq*lq) + (lr*lr) + (lr*lq);
        a = (long unsigned int)sqrt((double)a2);
        if (a*a != a2)
            continue;

        b2 = (lp*lp) + (lr*lr) + (lp*lr);
        b = (long unsigned int)sqrt((double)b2);
        if (b*b != b2)
            continue;

        // found r !!
        return r;
    }

    return 0;
}

/**
 * return triple (a,b,c) where a & b are p & q in the triple (p, q, r) we are looking for
 */
triple_t triple (int n, int m) {
    int a, b, c;

    a = m*m-n*n;
    b = 2*n*m+n*n;
    c = m*m+n*n+n*m;

    return (triple_t){a,b,c};
}

int main () {
    int n, m, p, q, r, s;
    triple_t t;
    long unsigned int sum;
    int pqr [MAX_VAL+1] = {};

    for (n = 1; n < 400; n++) {
        for (m = n+1; m < 600-n-2; m++) {
            t = triple(n, m);
            p = t.a;
            q = t.b;
            if (p > MAX_VAL || q > MAX_VAL)
                break;

            if ((r = find_r(p, q)) > 0) {
                if (pqr[r] > 0)
                    break;

                s = p+q+r;

                if (pqr[s] == 0) {
                    sum += (long unsigned int)s;
                    printf("n:%d, m:%d ==> p:%d, q:%d, r:%d = %d\n", n, m, p, q, r, p+q+r);
                }
                pqr[s]++;
            }
        }
    }

done:
    printf("sum: %lu\n", sum);

    return 0;
}
