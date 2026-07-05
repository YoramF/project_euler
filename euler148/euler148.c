/**
 * https://projecteuler.net/problem=148
 * 
 * First in order to avoid endless calculations and huge numbers we can represent the Pascal triange in the form
 * of binimial coefficien C(n,k);
 * There are very efficient ways to determine if C(n,k) is divisible by a number m. The method depends on whether 
 * m is a prime number or a composite number.
 * If m is a Prime(p), you can use Lucas's Theorem or Kummer's Theorem.Lucas’s Theorem: This is the most direct way to
 * check for divisibility. Write n and k in base p: 
 *  n = n_a*p^a +..+ n_1*p + n
 *  k = k_a*p^a +..+ k_1*p + k_0
 * Divisibility Rule: C(n,k) is divisible by p if and only if at least one base p digit of k is greater than the
 *  corresponding digit of n (i.e., k_i > n_i for some i).
 * Since we need to check of dividibility with 7 (which is a prime number) we can use this part of Lucas's Theorem.
 * All we need to do is to generate the above multiplications sums for m and k and check if we find corresponding digit of n (i.e., k_i > n_i for some i)
 * 
 * Since we need to check 10^9 rows it means 10^9/2*(2+(10^9-1)) checks (sum of Arithmetic series Sn=n/2*(a1*2+(n-1)*d))
 * that will take forever. Checking only the first alf of a raw will save only half of the time which is not enough.
 * There must be some another factor that needs to be taken into account...
 * 
 * Runing the above formula on 1000 lines it seems that there is a patten of how many C(n,k) are divisble by 7 and the
 * row number. If I find this pattern I just need to run through 10^9 rows and calculate the required sum.
 * 
 * It seems that I can use Lucas' Theorem again but this time to calculate how many elements in row n (couinting form 0..)
 * can't be divided by 7 and to subtract the number of elements in this row from that value.
 * f(n) = (a_k + 1)*(a_k-1 + 1)*...*(a_0 + 1) where a_k are the digits of n in base 7
 * 
 * Calculating each row is now much faster but still going through 10^9 row took
 * 1m5.449s
 * 
 * I guess there is a way to eliminate the need to calculate each row.
 * 
 * Solution:
 * in 1000000000 rows, there are 2129970655314432 of them are not divisable by 7

   real    1m5.449s
   user    1m5.435s
   sys     0m0.006s
 */

 #include <stdio.h>
 #include <stdbool.h>

 #define LASTLINE   1000000000
 
 /**
  * Check if C(n,k) is divisable by b
  */
 bool is_div_by_b (int n, int k, int b) {
    int nd, kd;

    while (n > 0 && k > 0) {
        nd = n % b;
        kd = k % b;

        // check the divisibility condition
        if (kd > nd)
            return true;
            
        // continue to check;
        n /= b;
        k /= b;
    }

    // if we get here it means C(n,k) is not divisable by b
    return false;
 }

 /**
  * return the number of elements in row r which are not divisibale by b
  * r = 0....n
  */
 int nd_by_b_in_row (int r, int b) {
    int nd = 1;
    int ak;

    while (r) {
        ak = r % b;
        nd *= (ak + 1);
        r /= b;
    }

    return nd;
 }

/**
 * Helping function to find the values patern based on line number
 */
void gen_values (int rows) {
    int n, k, isd_7, row_c1, row_c2, row_d;
    unsigned long int sum, nd_7_sum;

    sum = 0L;
    nd_7_sum = 0L;
    row_c1 = 0;
    row_c2 = 0;
    row_d = 1;
    
    for (n = 0; n < rows; n++) {
        isd_7 = 0;
        for (k = 0; k <= n; k++) {
            sum++;
            if (is_div_by_b(n,k, 7)) {
                isd_7++;
            }
        }
        nd_7_sum += (k - isd_7);
        row_c1++;
        row_c1 %= 7;
        if (row_c1 == 0) {
            printf("n: %d isd_7: %d, row_c1:%d, row_c2:%d, row_d:%d\n", n+1, isd_7, row_c1, row_c2, row_d);
            row_c2++;
            row_c2 %= 7;
            if (row_c2 == 0) {
                row_d++;
                row_d %= 7;
            }

        }
        else
            printf("n: %d isd_7: %d, row_c1:%d\n", n+1, isd_7, row_c1);        
    }

    printf("in %d rows, there are %lu entries, were %lu of them are not divisable by 7\n", n, sum, nd_7_sum);
 }

 int main () {
    long unsigned int sum = 0;
    int d;
    // gen_values(10000);
    for (int i = 0; i < LASTLINE; i++) {
        d = nd_by_b_in_row(i, 7);
        // printf("[%d] nd_7:%d\n", i, d);
        sum += d;
    }
    printf("in %d rows, there are %lu of them are not divisable by 7\n", LASTLINE, sum);

    return 0;
 }