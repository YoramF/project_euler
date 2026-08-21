/**
 * https://projecteuler.net/p1roblem=153
 * 
 * There are some rules that need to be followd:
 * 1) Integer n can have real integer divisors and Gaussian integers divisors
 * 2) Gaussian integer is of the form (a + bi) or (a - bi) where i is the imaginary number
 * 3) The set of standard rational prime factors is n = p_1^e_1*p_2^e_2...p_k^e_k {0<=e_m}
 * 4) Rules to form the Gaussian divisors:
 *  To factor n into Gaussian primes, decompose its rational prime factors based on their behavior
 *  a) The prime 2 (Ramified): 2 = -i(1+i)^2, so 2^k ~ (1+i)^{2k}.
 *  b) Primes p mod 4 == 3 (Inert): Primes like 3, 7, 11, 19 remain the same.
 *  c) Primes p mod 4 == 1 (Split): Primes like 5, 13, 17, 29 split into two distinct, conjugate Gaussian primes:
 *          p = (a+bi)(a-bi) {where } a^2 + b^2 == p
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <math.h>

#include <prime.h>

#define MAX_N 100000000

typedef struct factor {
   struct factor   *next;
   unsigned        prime;
   int             pwr;
   int             a;
   int             b;
} factor_t;

/**
 * Helper function to generate a random number between two numbers q & b
 */
unsigned int r_rand (unsigned int a, unsigned int b) {
    srand(time(NULL));
    int r = RAND_MAX;

    return (((unsigned int)rand() % (b - a + 1)) + a);
}

/**
 * Helper function to return (base^exp)%mod without the fear of large number overflow
 */
long unsigned modular_pow(unsigned base, unsigned exp, unsigned mod) {
    long unsigned res = 1L;
    long unsigned lbase, lmod;

    lbase = (long unsigned)base;
    lmod = (long unsigned)mod;
    base = base % mod;
    while (exp > 0L) {
        if (exp % 2L == 1L) res = (res * lbase) % lmod;
        lbase = (lbase * lbase) % lmod;
        exp /= 2L;
    }
    return res;
}

/**
 * Helper function that find x so that for a given prime p, x^2 == (p - 1) % p
 */
unsigned int find_modular_square_root_of_minus_one (unsigned int p) {
    unsigned int t_exp, r_exp;
    unsigned int a;
    long unsigned x;

    t_exp = (p - 1) / 2;
    r_exp = (p - 1) / 4;
   
    while (1) {
        a = r_rand(2, p - 1);

        // Compute a^((p-1)/2) mod p using fast modular exponentiation
        x = modular_pow(a, t_exp, p);
        
        // If the positive remainder is exactly p - 1, a is a non-residue
        if (x == (long unsigned)(p - 1))
            return (unsigned int)modular_pow(a, r_exp, p);
    }
}

/**
 * Hermite-Serret algorithm to find x & z so that for a given Prime, Prime == x^2 + z^2
 * The Prime must meet the following: Prime % 4 == 1
 * If Prime is not a valid prime return 0 otherwize update x and z
 */
int prime_factorization (unsigned int p, unsigned int *x, unsigned int *z) {
    double bound;
    unsigned int m_s_r, rem1, rem2, t;

    // first check that the prime argument is a valid one
    if ((p % 4) != 1)
        return 0;

    // Step 1: Find the modular square root z
    m_s_r = find_modular_square_root_of_minus_one(p);

    
    // Step 2: Establish the stopping bound
    bound = powr(p, 0.5);

    // Step 3: Run Euclidean algorithm tracking sequential remainders
    rem1 = p;
    rem2 = m_s_r;
    
    while (rem2 >= bound) {
        t = rem1;
        rem1 = rem2;
        rem2 = t % rem2;  
    }
       
    // x is the first remainder below the bound
    *x = rem2;

    // y is the next remainder in the sequence
    *z = rem1 % rem2;

    return 1;
}
        


/**
 * Generate a prime factors for a given integer n
 * Return a link list of factors
 */
factor_t *calc_factors (int n, unsigned *primes, unsigned ps) {
   int i;
   unsigned p, d;
   factor_t *factors = NULL, *l_factor = NULL, *p_factor;
   i = 0;
   while (i < ps && n > 0) {
       p = primes[i];
       d = 0;
       while (n % p == 0) {
           // check if new prime
           if (l_factor == NULL) {
               if ((l_factor = calloc(1, sizeof(factor_t))) == NULL) {
                   perror("Failed to allocate RAM for new factor");
                   exit (EXIT_FAILURE);
               }
               // check if this is the first factor in the list. if it does, update factors
               if (factors == NULL)
                   factors = l_factor;
               else
                   p_factor->next = l_factor;
               p_factor = l_factor;
               // update factor info
               l_factor->prime = p;
               l_factor->next = NULL;
           }
           d++;
           n /= p;
       }
       if (l_factor != NULL) {
           // current prime is done
           l_factor->pwr = d;
           l_factor = NULL;
       }
       i++;
   }
   return factors;
}

/**
 * Generate list of rational integer divisors
 * Return pointer to this list
 */
long unsigned sum_rational_dividors (factor_t *factors, unsigned n) {
    int pwr;
    unsigned div, pdiv;
    long unsigned sum = 0L;

    div = n;
    pdiv = factors->prime;
    for (pwr = 0; pwr <= factors->pwr; pwr++) {
        if (factors->next != NULL)
            sum += sum_rational_dividors(factors->next, div);
        else
            sum += div;

        div = n * pdiv;
        pdiv *= factors->prime;
    }

    return sum;
}

/**
 * Recuce list of factors to those which have gauissian primes
 */
factor_t *gaussian_primes (factor_t *factors) {
    factor_t *f_front, *f_back, *g_front = NULL, *g_back = NULL;

    f_front = factors;
    while (f_front !=  NULL) {
        f_back = f_front;
        f_front = f_front->next;
        if ((f_back->prime == 2) || ((f_back->prime % 4) == 1)) {
            // this prime has Gaussian prime
            if (g_back == NULL) {
                // first Gaussian prime
                g_front = f_back;
                g_back = f_back;
            }
            else {
                g_back->next = f_back;
                g_back = f_back;
            }
            g_back->next = NULL;
            if (g_back->prime > 2) {
                if (prime_factorization(g_back->prime, &(g_back->a), &(g_back->b)) == 0) {
                    fprintf(stderr, "Sent invalid prime numer %u to prime_factorization()\n", g_back->prime);
                    exit(EXIT_FAILURE);
                }
            }

        }
        else
            free(f_back);
    }

    return g_front;
}

/**
 * The 
 */


void p_factors (factor_t *factors, int factorization) {
   factor_t *f, *p_f;
   f = factors;
   while (f != NULL) {
       p_f = f;
       printf("prime: %u, pwr: %d ", f->prime, f->pwr);
       if (factorization)
            printf("a: %u, b: %u", f->a, f->b);
        printf("\n");
       f = f->next;
    //    free(p_f);
   }
}

void release_factors (factor_t *f) {
    factor_t *c_f;

    while (f != NULL) {
        c_f = f;
        f = f->next;
        free(c_f);
    }
}

int main () {
   int n;
   unsigned *primes, ps;
   factor_t *f;
   primes = gen_prime(MAX_N, &ps);
   while (1) {
        printf("Enter n: ");
        scanf("%d", &n);
        if (n < 2)
            break;
        f = calc_factors(n, primes, ps);
        printf("sum: %lu\n", sum_rational_dividors (f, 1));
        p_factors(f, 0);
        printf("filter gaussian primes:\n");
        f = gaussian_primes(f);
        p_factors(f, 1);
        release_factors(f);
   }
   return 0;
}