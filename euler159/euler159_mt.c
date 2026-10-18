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
* This brute force calculation can be accelerated by split it to smaller parallels.
* Running in 10 parallels makes it 10 times faster
* 
* s_mdrs: 14489159
* 
* real    0m12.515s
* user    1m53.011s
* sys     0m0.022s
* 
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <prime.h>
#include <async.h>

#define MAX             1000000
#define CHUNK           10000
#define PRIME_FACTORS   20         // max possible prime factors for value MAX is 20 (2^20==1048576)
#define THREADS         10

// Structure to hold a prime factor of a given number.
// Each real number can be represented as P1^k1*P2^k2*...*Pn^kn
typedef struct {
    int value;
    int count;
} prime_factor_t;

typedef struct {
    int base;                       // start calculation number
    int count;                      // amount of numbers to calculate
    int *g_num;                     // global last number in calculations
    int *g_mdrs_a;                  // address of global arry of mdrs
    int *mdrs_a;                    // local array of mdrs's
    unsigned int *primes;           // array of primes
    prime_factor_t *p_factors;      // local array of prime factors
    async_id_t async_id;            // we need that to allow callback to launch new worker
} async_arg_t;

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
void calculate_mdrs(int num, int *mdrs_a, int ind, prime_factor_t *p_factors, int num_unique, int **blocks, int block_count) {
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
    
    if (mdrs_a[ind] < mdrs)
        mdrs_a[ind] = mdrs;
}

// Forward declaration
void generate_partitions(int num, int *mdrs_a, int ind, prime_factor_t *p_factors, int num_unique, int **blocks, int block_count);

// Helper to recursively build valid sub-multiset blocks
void build_block(int num, int *mdrs_a, int ind, prime_factor_t *p_factors, int num_unique, int **blocks, int block_count, 
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
            generate_partitions(num, mdrs_a, ind, p_factors, num_unique, blocks, block_count + 1);

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
        build_block(num, mdrs_a, ind, p_factors, num_unique, blocks, block_count, current_block, u_idx + 1, anchor_idx);
    }
    current_block[u_idx] = 0;
}

// Recursive partition generator
void generate_partitions(int num, int *mdrs_a, int ind, prime_factor_t *p_factors, int num_unique, int **blocks, int block_count) {
    int anchor_idx, *current_block;

    if (is_empty(p_factors, num_unique)) {
        calculate_mdrs(num, mdrs_a, ind, p_factors, num_unique, blocks, block_count);
        return;
    }

    anchor_idx = find_anchor(p_factors, num_unique);
    current_block = (int *)calloc(num_unique, sizeof(int));

    build_block(num, mdrs_a, ind, p_factors, num_unique, blocks, block_count, current_block, 0, anchor_idx);

    free(current_block);
}

//  async worker
void ex_func (void *arg, void *res) {
    async_arg_t *l_arg = (async_arg_t *)arg;
    int *mdrs;
    int s_mdrs = 0, p_count;
    int m_factors;
    int **factors;

    // allocate local array for mdrs
    if ((mdrs = calloc(l_arg->count, sizeof(*mdrs))) == NULL) {
        perror("Failed to allocate RAM for local mdrs");
        exit(EXIT_FAILURE);
    }
    l_arg->mdrs_a = mdrs;

    for (int n = l_arg->base, i = 0; i < l_arg->count; n++, i++) {
        // gen factors for n which is not prime
        if (l_arg->g_mdrs_a[n] == 0) {
            memset(l_arg->p_factors, 0, PRIME_FACTORS * sizeof(prime_factor_t));
            p_count = gen_prime_factors(n, l_arg->primes, l_arg->p_factors);
            m_factors = max_factors(l_arg->p_factors, p_count);

            // allocate RAM for factors subset
            if ((factors = (int **)malloc(m_factors * sizeof(int *))) == NULL) {
                perror("Failed to allocate RAM for factors subset");
                exit(EXIT_FAILURE);
            }

            // now call generate_subsets and partitions
            generate_partitions(n, mdrs, i, l_arg->p_factors, p_count, factors, 0);

            // free factors
            free(factors);
        }
        else
            // we must have this assignment so that in cb_func, the prime drs values which were
            // initialized at the beginning will not be zerroed.
            l_arg->mdrs_a[i] = l_arg->g_mdrs_a[n];
    }
}

// callback function
void cb_func (void *arg, void *res) {
    async_arg_t *l_arg = (async_arg_t *)arg;
    int add;
    
    // copy local mdrs to global mndr_a
    for (int i = 0, n = l_arg->base; i < l_arg->count; i++, n++)
        l_arg->g_mdrs_a[n] = l_arg->mdrs_a[i];

    // release local mdrs
    free(l_arg->mdrs_a);

    add = MAX - *(l_arg->g_num) > CHUNK? CHUNK:  MAX - *(l_arg->g_num) + 1;    

    // if we are not done yet, launch new worker
    if (add > 0) {
        l_arg->base = *(l_arg->g_num);
        l_arg->count = add;

        // update global g_num
        *(l_arg->g_num) += add;
        
        // launch new worker
        async_launch(&ex_func, &cb_func, arg, arg, arg, l_arg->async_id);
    }
}


int main () {
    unsigned int *primes, ps;
    int *mdrs_a;
    int s_mdrs = 0;
    async_id_t async_id;
    int g_num = 1;
    int n;
    async_arg_t *args[THREADS];

    primes = gen_prime(MAX, &ps);

    // init async env with single callback to force sync global updates
    async_id = async_init(THREADS, 1);

    if ((mdrs_a = (int *)calloc(MAX, sizeof(*mdrs_a))) == NULL) {
        perror("Failed to allocate RAM for mdrs_a");
        exit(EXIT_FAILURE);
    }

    // init all primes locations inside mdrs_a
    for (n = 0; n < ps; n++)
        mdrs_a[primes[n]] = drs(primes[n]);

    for (n = 0; n < THREADS; n++) {
        int base;
        async_arg_t *arg;
        prime_factor_t *p_factors;

        if ((arg = malloc(sizeof(*arg))) == NULL) {
            perror("Failed to allocate RAM for arg");
            exit(EXIT_FAILURE);
        }

        if ((p_factors = (prime_factor_t *)malloc(PRIME_FACTORS * sizeof(*p_factors))) == NULL) {
            perror("Faied to allocate RAM for p_factors");
            exit(EXIT_FAILURE);
        }

        if (g_num == 1) {
            arg->base = 2;
            arg->count = CHUNK - 1;
        }
        else {
            arg->base = g_num;
            arg->count = CHUNK;
        }

        g_num += CHUNK;
        arg->g_mdrs_a = mdrs_a;
        arg->g_num = &g_num;
        arg->p_factors = p_factors;
        arg->primes = primes;
        arg->async_id = async_id;

        //store arg for later release
        args[n] = arg;
    }

    for (n = 0; n < THREADS; n++)
        async_launch(&ex_func, &cb_func, args[n], args[n], args[n], async_id);

    // wait for async execution to terminate and release the env
    async_terminate(async_id);

    // release all pre-allocated ram
    for (n = 0; n < THREADS; n++) {
        free(args[n]->p_factors);
        free(args[n]);
    }

    for (n = 2; n < MAX; n++) {
        s_mdrs += mdrs_a[n];
    }

    printf("s_mdrs: %d\n", s_mdrs);
    free(mdrs_a);

    return 0;
}
