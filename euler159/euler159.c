/**
* https://projecteuler.net/problem=159
* 
* Using brute force approach.
* drs(int n) return the Digital Root Sum of n.
* gen_prime_factors(int n, unsigned int primes) takes an interger n and using primes array, generates the prime
* factors of integer n in the form of P1^k1*P2^k2*...*Pn^kn
* Using this prime factors list we can generate a list of primes (p1,p1...p1,p2...p2, ... pn...pn) and partition
* this set into all combinations of subsets of the form ((p1,p1,p2..),(p1,p2,..), p1,p2,....).
* Per combination we can calculate its drs by first calculating the drs of each subset and then add them with the
* drs of the remaining numbers in the bigger set. This sum is the mdrs of number n for a given subset combination.
* We need to store the biggest mdrs and then sum all mdrs for 1<n<1000000.
* This brute force calculation can be accelerated by split it to smalled parallels.
* 
* s_mdrs: 14489159
* 
* real    2m20.369s
* user    2m20.348s
* sys     0m0.002s
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <prime.h>

#define MAX 1000000
#define PRIME_FACTORS 20         // max possible prime factors for value MAX is 20 (2^20==1048576) 

// Structure to hold a prime factor of a given number.
// Each real number can be represented as P1^k1*P2^k2*...*Pn^kn
typedef struct {
    int value;
    int count;
} prime_factor_t;

// return the Digital Root Sum of n
int drs (int n) {
    int r = 0, d;

    while (n) {
        d = n % 10;
        r += d;
        n /= 10;
        if (r >= 10)
            r = drs(r);
    }

    return r;
}


// per given, generate the list of prime_factors
int gen_prime_factors (int num, unsigned int *primes, prime_factor_t *p_factors) {
    unsigned int p;
    int p_ind, f_ind, factors;

    f_ind = 0;
    p_ind = 0;
    factors = 0;
    while (num > 1) {
        while (num % primes[p_ind] == 0) {
            // check if first time for this factor
            if (p_factors[f_ind].value == 0) {
                p_factors[f_ind].value = primes[p_ind];
                factors++;
            }
            p_factors[f_ind].count++;
            num /= primes[p_ind];
        }

        if (p_factors[f_ind].value > 0)
            // we are done with current prime factor
            f_ind++;

        // move to next prime
        p_ind++;
    }

    return f_ind;
}

// find maximum number of factors for number num
int max_factors (prime_factor_t *p_factors, int p_count) {
    int m_factors = 0;
    for (int i = 0; i < p_count; i++)
        m_factors += p_factors[i].count;

    return m_factors;
}

// Check if all elements have been fully partitioned
bool is_empty(prime_factor_t *p_factors, int num_unique) {
    for (int i = 0; i < num_unique; i++) {
        if (p_factors[i].count > 0) return false;
    }
    return true;
}

// Find the index of the smallest available element to anchor the block
int find_anchor(prime_factor_t *p_factors, int num_unique) {
    for (int i = 0; i < num_unique; i++) {
        if (p_factors[i].count > 0) return i;
    }
    return -1;
}

// Print a completed partition
void calculate_mdrs(int num, int *mdrs_a, prime_factor_t *p_factors, int num_unique, int **blocks, int block_count) {
    int mdrs;
    int s_mul;

    mdrs = 0;

    for (int i = 0; i < block_count; i++) {

        s_mul = 1;
        for (int u = 0; u < num_unique; u++) {
            for (int c = 0; c < blocks[i][u]; c++) {
                s_mul *= p_factors[u].value;
            }
        }
        mdrs += drs(s_mul);
    }
    
    if (mdrs_a[num] < mdrs)
        mdrs_a[num] = mdrs;
}

// Forward declaration
void generate_partitions(int num, int *mdrs_a, prime_factor_t *p_factors, int num_unique, int **blocks, int block_count);

// Helper to recursively build valid sub-multiset blocks
void build_block(int num, int *mdrs_a, prime_factor_t *p_factors, int num_unique, int **blocks, int block_count, 
                 int *current_block, int u_idx, int anchor_idx) {
    int *stored_block;
    int min_c;
    int max_c;
    int i;

    if (u_idx == num_unique) {
        // A valid block must contain at least one instance of the anchor element
        if (current_block[anchor_idx] > 0) {

            // Allocate a dedicated copy of the block for this branch
            if ((stored_block = (int *)malloc(num_unique * sizeof(int))) == NULL) {
                perror("Failed to allocate RAM for stored block");
                exit(EXIT_FAILURE);
            }

            for (i = 0; i < num_unique; i++)
                stored_block[i] = current_block[i];

            // Subtract block from remaining counts
            for (i = 0; i < num_unique; i++) {
                p_factors[i].count -= stored_block[i];
            }

            blocks[block_count] = stored_block;
            generate_partitions(num, mdrs_a, p_factors, num_unique, blocks, block_count + 1);

            // Restore counts and free the block copy
            for (i = 0; i < num_unique; i++) {
                p_factors[i].count += stored_block[i];
            }

            free(stored_block);
        }
        return;
    }

    min_c = (u_idx == anchor_idx) ? 1 : 0;
    max_c = p_factors[u_idx].count;

    for (int c = min_c; c <= max_c; c++) {
        current_block[u_idx] = c;
        build_block(num, mdrs_a, p_factors, num_unique, blocks, block_count, current_block, u_idx + 1, anchor_idx);
    }
    current_block[u_idx] = 0;
}

// Recursive partition generator
void generate_partitions(int num, int *mdrs_a, prime_factor_t *p_factors, int num_unique, int **blocks, int block_count) {
    int anchor_idx, *current_block;

    if (is_empty(p_factors, num_unique)) {
        calculate_mdrs(num, mdrs_a, p_factors, num_unique, blocks, block_count);
        return;
    }

    anchor_idx = find_anchor(p_factors, num_unique);
    current_block = (int *)calloc(num_unique, sizeof(int));

    build_block(num, mdrs_a, p_factors, num_unique, blocks, block_count, current_block, 0, anchor_idx);

    free(current_block);
}


int main () {
    int n;
    int *mdrs_a;
    int s_mdrs = 0, p_count;
    unsigned int *primes, ps;
    prime_factor_t *p_factors;
    int m_factors;
    int **factors;

    primes = gen_prime(MAX, &ps);

    if ((mdrs_a = (int *)calloc(MAX, sizeof(*mdrs_a))) == NULL) {
        perror("Failed to allocate RAM for mdrs_a");
        exit(EXIT_FAILURE);
    }

    if ((p_factors = (prime_factor_t *)malloc(PRIME_FACTORS * sizeof(*p_factors))) == NULL) {
        perror("Faied to allocate RAM for p_factors");
        exit(EXIT_FAILURE);
    }

    // init all primes locations inside mdrs_a
    for (n = 0; n < ps; n++)
        mdrs_a[primes[n]] = drs(primes[n]);

    for (n = 2; n < MAX; n++) {
        // gen factors for n which is not prime
        if (mdrs_a[n] == 0) {
            memset(p_factors, 0, PRIME_FACTORS * sizeof(*p_factors));
            p_count = gen_prime_factors(n, primes, p_factors);
            m_factors = max_factors(p_factors, p_count);

            // allocate RAM for factors subset
            if ((factors = (int **)malloc(m_factors * sizeof(int *))) == NULL) {
                perror("Failed to allocate RAM for factors subset");
                exit(EXIT_FAILURE);
            }

            // now call generate_subsets and partitions
            generate_partitions(n, mdrs_a, p_factors, p_count, factors, 0);

            // free factors
            free(factors);
        }
    }

    for (n = 2; n < MAX; n++) {
        s_mdrs += mdrs_a[n];
    }

    printf("s_mdrs: %d\n", s_mdrs);
    free(mdrs_a);

    return 0;
}
