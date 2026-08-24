/**
 * https://projecteuler.net/problem=153
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
#include <imaginary.h>

#define MAX_N 100000000

typedef struct real_factor {
   struct real_factor   *next;
   unsigned             prime;
   int                  pwr;
} real_factor_t;

typedef struct complex_factor {
    struct complex_factor   *next;
    complex_t               comp;
    int                     pwr;
} complex_factor_t;

typedef struct {
    complex_factor_t    *head;
    complex_factor_t    *tail;
} complex_prime_factor_t;

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
int prime_factorization (unsigned int p, int *a, int *b) {
    double bound;
    unsigned int m_s_r, rem1, rem2, t;

    // check special case for prime == 2
    if (p == 2) {
        *a = 1;
        *b = 1;
        return 1;
    }

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

    *a = rem2;
    *b = rem1%rem2;

    return 1;
}      

/**
 * Generate a real prime factors for a given integer n
 * Return a link list of real_factors
 */
real_factor_t *calc_real_factors (int n, unsigned *primes, unsigned ps) {
   int i;
   unsigned p, d;
   real_factor_t *factors = NULL, *l_factor = NULL, *p_factor;
   i = 0;
   while (i < ps && n > 0) {
       p = primes[i];
       d = 0;
       while (n % p == 0) {
           // check if new prime
           if (l_factor == NULL) {
               if ((l_factor = calloc(1, sizeof(real_factor_t))) == NULL) {
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
 * Return sum of all divisors
 */
long unsigned sum_rational_dividors (real_factor_t *factors, unsigned n) {
    int pwr;
    unsigned div, pdiv;
    long unsigned sum = 0L;

    if (factors != NULL) {
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
    }

    return sum;
}

/**
 * Generate list of complex divisors
 * Return complex sum of all complex divisors 
 */
complex_t sum_complex_dividors (complex_factor_t *factors, complex_t c) {
    int pwr;
    complex_t comp, p_comp;
    complex_t c_sum, t_sum;
    char str1[30], str2[30];

    set_comp(c_sum, 0, 0);

    if (factors != NULL) {
        comp = c;
        p_comp = factors->comp;
            printf("c: %s, p_comp: %s, pwr: %d\n", c_print(c, str1), c_print(p_comp, str2), factors->pwr);
        for (pwr = 0; pwr <= factors->pwr; pwr++) {
            if (factors->next != NULL)
                c_sum = c_add(c_sum, sum_complex_dividors(factors->next, comp));
            else
                c_sum = c_add(c_sum, comp);

            comp = c_mul(c, p_comp);
            p_comp = c_mul(p_comp, factors->comp);
        }
    }

    return c_sum;
}

/**
 * Recuce list of factors to those which have gauissian primes
 */
complex_factor_t *gaussian_primes (real_factor_t *factors) {
    real_factor_t *f_front, *f_back;
    complex_factor_t *g_front = NULL, *g_back = NULL, *g_new;
    int a, b;

    f_front = factors;
    while (f_front !=  NULL) {
        f_back = f_front;
        f_front = f_front->next;
        if ((f_back->prime == 2) || ((f_back->prime % 4) == 1)) {
            // this prime has Gaussian prime
            if (prime_factorization(f_back->prime, &a, &b) == 0) {
                fprintf(stderr, "Sent invalid prime numer %u to prime_factorization()\n", f_back->prime);
                exit(EXIT_FAILURE);
            }
            // allocate new complex_factor node
            if ((g_new = malloc(sizeof(complex_factor_t))) == NULL) {
                perror("Failed to allocate RAM for complex factor");
                exit(EXIT_FAILURE);
            }
            if (g_back == NULL) {
                // first Gaussian prime
                g_front = g_new;
            }
            else {
                g_back->next = g_new;
            }

            set_comp(g_new->comp, a, b);
            g_new->next = NULL;
            if (f_back->prime == 2)
                g_new->pwr = f_back->pwr * 2;
            else
                g_new->pwr = f_back->pwr;
            g_back = g_new;

            // allocate secod complex factor for the negative imaginary

            if ((g_new = malloc(sizeof(complex_factor_t))) == NULL) {
                perror("Failed to allocate RAM for complex factor");
                exit(EXIT_FAILURE);
            }

            // in case prime > 2 we need additional complex factor with negative im
            if (f_back->prime == 2)
                set_comp(g_new->comp, 0, -1);
            else
                set_comp(g_new->comp, a, -b);

            g_new->pwr = f_back->pwr;
            g_new->next = NULL;
            g_back->next = g_new;
            g_back = g_new;              
        }
    }

    return g_front;
}

/**
 * The 
 */


void p_real_factors (real_factor_t *factors) {
   real_factor_t *f, *p_f;
   f = factors;
   while (f != NULL) {
       p_f = f;
       printf("prime: %u, pwr: %d\n", f->prime, f->pwr);
       f = f->next;
   }
}

void p_complex_factors (complex_factor_t *factors) {
   complex_factor_t *f, *p_f;
   char str[30];

   f = factors;
   while (f != NULL) {
       p_f = f;
       printf("comp: %s, pwr: %d\n", c_print(f->comp, str), f->pwr);
       f = f->next;
   }
}

real_factor_t *release_real_factors (real_factor_t *f) {
    real_factor_t *c_f;

    while (f != NULL) {
        c_f = f;
        f = f->next;
        free(c_f);
    }

    return NULL;
}

complex_factor_t *release_complex_factors (complex_factor_t *f) {
    complex_factor_t *c_f;

    while (f != NULL) {
        c_f = f;
        f = f->next;
        free(c_f);
    }

    return NULL;
}

int main () {
   int n;
   unsigned *primes, ps;
   real_factor_t *f;
   complex_factor_t *cf;
   complex_prime_factor_t *cpf;
   primes = gen_prime(MAX_N, &ps);
   char str[30];
   complex_t one;

   // allocate array of prime factors poiters
   if ((cpf = malloc(ps * sizeof(complex_prime_factor_t))) == NULL) {
        perror("Failed to allocate RAM for array of complex primes factors");
        exit(EXIT_FAILURE);
   }

   set_comp_from_int(one, 1);
   printf("primes: %u\n", ps);
   while (1) {
        printf("Enter n: ");
        scanf("%d", &n);
        if (n < 2)
            break;
        f = calc_real_factors(n, primes, ps);
        printf("sum: %lu\n", sum_rational_dividors (f, 1));
        p_real_factors(f);
        printf("filter gaussian primes:\n");
        cf = gaussian_primes(f);
        p_complex_factors(cf);
        printf("sum of complex prime factors: %s\n", c_print(sum_complex_dividors(cf, one), str));
        f = release_real_factors(f);
        cf = release_complex_factors(cf);
   }
   return 0;
}