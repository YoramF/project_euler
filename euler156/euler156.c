/**
 * https://projecteuler.net/problem=156
 * 
 * Key Properties & Upper Bound ProofGrowth Rate Comparison:
 * For n = 10^k - 1, the exact count of any non-zero digit is f(10^k - 1, d) = k * 10^(k-1).
 * 
 * To understand why f(10^k-1, d) = k * 10^(k-1), consider all the numbers written out with leading zeros up to k 
 * digits long. The range of numbers from 0 to 10^k-1 contains exactly 10^k numbers:
 * {0, 1, 2, 3, ... ,10^k-1}. If we Pad every single number with leading zeros so that each number has exactly k 
 * digits (for example, for k=3, write 0 as 000, 5 as 005, and 99 as 099), we have: Total numbers: 
 * 10^k Digits per number: k Total digit positions filled: k ... 10^k.
 * Symmetry Argument By symmetry, every digit from 0 through 9 appears with equal frequency across all k ... 10^k
 * positions. Since there are 10 possible digits (0, 1, 2, 3, 4, 5, 6, 7, 8, 9), the count for any specific digit d
 * is: {Count of digit } d = {Total digit positions} / 10 = k * 10^k / 10 = k * 10^(k-1).
 * 
 * 
 * At k = 10 (n ~ 10^10: f(10^10-1, d) = 10 * 10^9 = 10^10.
 *  Here f(n,d) and n are of comparable size.
 *  At k = 11 (n ~ 10^11): f(10^(11-1), d) = 11 * 10^10 = 1.1 * 10^11.
 *  The function value grows faster than n.
 *  Beyond 10^11, f(n, d) > n almost everywhere. By analyzing the lower bound of f(n, d) for n >= 10^11, no solutions
 *  f(n, d) = n exist for n >= 10^11.
 * 
 * Stopping Condition:
 *  Search for solutions strictly in the interval 1 <= n < 10^11.
 *  Efficient Search Strategy (Divide and Conquer) A linear scan up to 10^11 is far too slow.
 *  Because f(n, d) is monotonically non-decreasing, use recursive interval pruning over [A, B]:
 *   Calculate L = f(A, d) and R = f(B, d).
 *  Pruning Rule: 
 *   If R < A or L > B, then no solution f(n, d) = n can exist within [A, B]. Stop exploring this branch.
 *   If the interval cannot be pruned, split [A, B] into sub-intervals (e.g., at the midpoint or per digit boundary) 
 *    and recurse down to A = B.
 * 
 *  sum: 21295121502550
 *  
 *  real    0m0.037s
 *  user    0m0.032s
 *  sys     0m0.005s
 * 
 */

#include <stdio.h>

#define MAX         100000000000    // 10^11
#define INC         100000000       // best inerval based on testing other inervals

/**
 * f(n,d).This function calculate how many times digit d appears in all numbers from 1..n
 * To do that, we should consider the fact that for n = 10^k - 1, f(n, d) = k * 10^(k - 1), where k is the number of
 * digits in n.
 * To calculate f(n, d) the total number of times a non-zero digit d in {1, ..., 9\} appears in all integers from 
 * 1 to n you sum the occurrences of d at each digit position (ones, tens, hundreds, etc.) independently.
 * Digit-by-Digit Counting FormulaFor a given position with weight m (where m = 1 for ones place, m = 10 for tens place,
 * m = 100 for hundreds place, etc.):
 * Split n into three parts relative to m: 
 *  {higher}: floor(n / (m * 10)) (the digits to the left)
 *  {current}: floor(n / m) % 10 (the digit at position m)
 *  {lower}: n mod m (the digits to the right)
 * The total number of times digit d appears at position m depends on the value of {current}:
 *  If {current} < d: {Count} = {higher} * m
 *  If {current} == d: {Count} = {higher} * m + {lower} + 1
 *  If {current} > d: {Count} = ({higher} + 1) * m
 * 
 * Sum the counts across all digit positions m = 1, 10, 100, .. <= n to get f(n, d).
 */
long unsigned f (long unsigned n, int d) {
   long unsigned count = 0L;
    long unsigned m = 1L;
    long unsigned higher, current, lower;

    while (m <= n) {
        higher = n / (m * 10L);
        current = (n / m) % 10L;
        lower = n % m;

        if (current < d)
            count += higher * m;
        else if (current == d)
            count += higher * m + lower + 1;
        else
            count += (higher + 1) * m;

        m *= 10;
    }

    return count;    
}

/**
 * Pruning function
 * In range between A..B if f(A,d) < A or f(B,d) > B this range can be pruned.
 * If f(n,d) is within the range, use binary splitting to reduce the amount of tests
 */
long unsigned prune (long unsigned low, long unsigned high, int d) {
    long unsigned L, R, mid;

    L = f(low, d);

    // stop condition for the recursive search
    if (low == high) {
        // return reacl value only if f(n,d) == n
        if (L == low) {
            return L;
        }
        // otherwise we managed to find a valid value for f(n,d) but it is not equal n. Return 0
        else
            return 0L;
    }

    R = f(high, d);
    mid = (high - low) / 2;

    // check if falid value for f(n,d) can be found inside the given range
    if (L > high || R < low)
        return 0L;
    else
        // keep searching but first split the range into two ranges
        return prune((low + mid + 1), high, d) + prune(low, (low + mid), d);
}

/**
 * To speen the search, run the prune functions on intervals of size INC. This will reduce the
 * overall number of tests even more
 */
long unsigned check_intervals (int d) {
    long unsigned p_sum = 0L, n, max;

    n = 1;
    while (n < MAX) {
        max = n + INC;
        if (max > MAX)
            max = MAX;
        p_sum += prune(n, max, d);
        n = max + 1;
    }
    return p_sum;
}

int main () {
    long unsigned sum = 0L, p;

    for (int d = 1; d < 10; d++) {
        p = check_intervals(d);
        sum += p;
    }

    printf("sum: %lu\n", sum);

    return 0;
 }