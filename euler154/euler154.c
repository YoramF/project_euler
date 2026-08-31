/**
 * https://projecteuler.net/problem=154
 * 
 * The coefficiants of (x+y+z)^n are sum of n!/(k1!*k2!*k3!) where k1+k2+k3 = n
 * For any combination of k1,k2,k3 the coefficiant can be caluclates as C(n,k1)*C(n-k1,y) where C(n,m) is n!/(m!*(n-m)!)
 * In order to keep the calculation values as small as possible we need to take into account that 
 * we are looking only for coefficiants which are multiply of 10^12 we can use Kummer's theorem to check if C(n,m) is
 * a multiplication of 10^12 without evaluating the real value of C(n,m).
 * 
 * Solution:
 * Coefficients: 479742450
 * 
 * real    9m47.142s
 * user    9m47.028s
 * sys     0m0.005s
 */

#include <stdio.h>

#define MAX_VAL         200000
#define PWR             12           


/**
 * return 1 bit counts of an integer
 */
int bit_count (long unsigned n) {
    int c = 0;
    while (n) {
        c += n & 1;
        n >>= 1;
    }
    return c;
}

/**
 * Use Kummer's theorem to find the largest power of prime p that divides C(n,x) where
 * A = x, B = n-x and p is the base to calculate carries for A+B
 * For base 2 we use optimized binary calculation
 */
int count_carries_base_p(int A, int B, int p) {
    int carries = 0;
    int carry = 0;

    // check special case if p == 2
    if (p == 2) {
        return bit_count((long unsigned)A) + bit_count((long unsigned)B) - bit_count((long unsigned)(A + B));
    }
    
    while (A > 0 || B > 0 || carry > 0) {
        int digit_A = A % p;
        int digit_B = B % p;
        
        int sum = digit_A + digit_B + carry;
        if (sum >= p) {
            carries++;
            carry = 1;
        } else {
            carry = 0;
        }
        
        A /= p;
        B /= p;
    }
    
    return carries;
}

/**
 * check if n!/(x!*y!*z!) is a multiply of 10^PWR (10^12)
 * Using Kummer's theorem we basically need to find:
 * since we are looking at a multiply of 10, the two primes that we need to use are 2 and 5 and they will be used
 * as the basses for the count_carries_base_p() function.
 * since we use C(n,x)*C(n-x,y) to calculate n!/(x!*y!*z!), for the Kummer's theorem, we need to count cariers of
 * V2 = v21(C(n,x)) + v22(C(n-x,y)) anc V5 = v51(C(n,x)) + v52((C(n-x,y))) and both V2 and V5 must be greater than PWR
 * for this problem PWR is 12 (10^12), Note that the cariers in our case are the number of trailing Zeros in the result
 * of n!/(x!*y!*z!) and the number must be >= 12.
 * Note that C(n,x) is sent to count_carries_base_p() with A=x, B=n-x
 * return 1 if the value is a multiply of 10^PWR or 0 if not
 */
int kummer_calc (int n, int x, int y, int pwr) {
    int v21, v22, v51, v52;
    int r = 0;

    // we will start with base 5 and only if it match the conditions we will proceed to base 2
    // because checking base 5 will fail sooner than base 2
    v51 = count_carries_base_p(x, n-x, 5);
    v52 = count_carries_base_p(y, n-x-y, 5);


    // check if base 5 is ok
    if ((v51 + v52) >= pwr) {
        v21 = count_carries_base_p(x, n-x, 2);
        v22 = count_carries_base_p(y, n-x-y, 2);
        if ((v21 + v22) >= pwr)
            r = 1;
    }

    return r;
}

/**
 * return number of terms given a combination of x,y,z
 */
int terms (int x, int y, int z) {
    int xy, xz, yz;

    xy = (int)(x == y);
    xz = (int)(x == z);
    yz = (int)(y == z);

    switch (xy + xz + yz) {
        case 0: return 6;
        case 1: return 3;
        case 3: return 1;
    }
}

/**
 * Generate all coefficients combinations so that x+y+z=n
 * Per combination check if n!/(x!*y!*z!) is a multiply of 10^12 using Kummer's theorem.
 */
long int comb (int n) {
    int x, y, z, t, c;
    int  sum = 0;

    for (x = n; x >= n/3; x--) {
        y = n - x;
        if (y > x)
            y = x;

        z = n - x - y;

        while (y >= z) {
            c = kummer_calc(n, x, y, PWR);
            t = terms(x, y, z);
            if (c > 0)
                sum += t;

            y--;
            z++;
        }
    }

    return sum;
}



int main () {
    int sum;

    sum = comb(MAX_VAL);
    printf("Coefficients: %d\n", sum);

    return 0;
}