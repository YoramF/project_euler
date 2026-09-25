/*
https://projecteuler.net/problem=110

The approach is the opposite way of problem 108, but still using brute force approach. 
Going backward, we know that the number we are looking for is a number that square must are at least 4M factor pairs, or
at least 8M factors.
Finding these factors (f1, f2, fn) and applying them as following:
2^(f1-1)*3^(f2-1)*5^(f3-1)*...*pn(fn-1) will be the smallest number were its square root is the number we are
looking for.  
The challenge with this approach is to set the stopping condition on the number of factors we are iterating over.
If we stop too soon, the number we find may not be the smallest one.

I needed several trials to get a good stopping condition to get the correct answer.
Not knowing the answer or close to it, I used gmp library to handle big numbers. Eventually the correct answer
could have been found using long unsigned int.

I read other solutions in the forum that took less than a second, but could not understand the logic behind tham
unless you know the solution in advance. So I stay with my poor algorithm :)

Solution:
The least value of n for which the number of distinct solutions exceeds four million, is: 9350130049860600, distinct solutions: 4018613

real    38m20.090s
user    38m4.997s
sys     0m13.366s

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>
#include <gmp.h>

#include <prime.h>

#ifdef DEBUG
#include "../../timer/timer.h"
#endif

#define MAX_PRIMES  100
#define MAX_FACTORS 200
#define REPEATS 100000

// define a variable size factorial structure 
typedef struct {
    int n_items;
    unsigned int factors[];
} FACTORIALS;

typedef struct _set {
    unsigned int count;
    FACTORIALS *fc;
    struct _set *next;
} SET;

#ifdef DEBUG
typedef struct {
    FACTORIALS **fc;
    unsigned int *n;
} TIMER_S;
#endif

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


void calc_factors(FACTORIALS *fc, unsigned int *primes, unsigned int mp, mpz_t r) {
    unsigned int pwr;
    int fi;
    mpz_t p, n;

    mpz_set_ui(r, 1);
    mpz_init(p);
    mpz_init(n);

    for (fi = 0; fi < mp && fi < fc->n_items; fi++) {
        pwr = fc->factors[fi]-1;
        mpz_set_ui(p, primes[fi]);
        mpz_pow_ui(n, p, pwr);
        mpz_mul(r, r, n);
    }

    mpz_clear(p);
    mpz_clear(n);
}

int q_func (const void *e1, const void *e2) {
    unsigned int *e1_, *e2_;

    e1_ = (unsigned int *)e1;
    e2_ = (unsigned int *)e2;

    return (*e2_ - *e1_);
}

#ifdef DEBUG
void print_fc (FACTORIALS *fc) {
    int i;
    printf("( ");
    for (i = 0; i < fc->n_items; i++)
        printf("%u ", fc->factors[i]);
    printf(")\n");
}
#endif

// compare two elements
int set_is_eq (const void *e_item, const void *n_item) {
    FACTORIALS *e1, *e2;
    int r = 0;

    e1 = (FACTORIALS *)e_item;
    e2 = (FACTORIALS *)n_item;

    if (e1->n_items == e2->n_items) {
        return memcmp(&e1->factors, &e2->factors, e1->n_items*sizeof(unsigned int));
    }

    return (e1->n_items - e2->n_items);
}

FACTORIALS *dup_fc (FACTORIALS *fc) {
    FACTORIALS *n_fc;

    if ((n_fc = malloc(sizeof(FACTORIALS)+fc->n_items*sizeof(unsigned int))) == NULL) {
        perror("Failed to reallocate RAM for updated fc\n");
        exit (EXIT_FAILURE);        
    }

    // copy fc to nfc
    memcpy(n_fc, fc, sizeof(FACTORIALS)+fc->n_items*sizeof(unsigned int));
    return n_fc;
}

// insert new fc into set only if it is new fc
int set_insert (SET *set, FACTORIALS *fc) {
    SET *set_n = set->next, *set_b = NULL;
    SET *s_fc;
    int r;

    if ((s_fc = malloc(sizeof(SET))) == NULL) {
        perror("Failed to allocate RAM for new SET element\n");
        exit (EXIT_FAILURE);
    }

    s_fc->fc = dup_fc(fc);
    s_fc->next = NULL;

    qsort(&s_fc->fc->factors, fc->n_items, sizeof(unsigned int), &q_func);

    while (set_n != NULL) {
        if ((r = set_is_eq(set_n->fc, s_fc->fc)) == 0) {
            // fc is already in set. 
            free(s_fc->fc);
            free(s_fc);
            return 0;
        }
        else if (r > 0) {
            set->count++;
            s_fc->next = set_n;
            if (set_b != NULL)
                set_b->next = s_fc;
            else
                set->next = s_fc;
            return 1;
        }
        set_b = set_n;
        set_n = set_n->next;
    }

    // got here if set is empty, or fc is bigger than all elements in set
    if (set->next == set_n)
        set->next = s_fc;
    else {
        set_b->next = s_fc;
    }

    set->count++;
    return 1;
}

#ifdef DEBUG
void print_factorials_sets (SET *fs) {
    int i = 1;

    fs = fs->next;
    while (fs != NULL) {
        printf("[%d] ",i);
        print_fc(fs->fc);
        i++;
        fs = fs->next;
    }
}
#endif

// this fc will be used for the recursive gen_factorials
FACTORIALS *init_fc () {
    FACTORIALS *fc;

    if ((fc = malloc(sizeof(FACTORIALS)+MAX_FACTORS*sizeof(unsigned int))) == NULL) {
        perror("Failed to allocate RAM for updated common fc\n");
        exit (EXIT_FAILURE);
    }

    fc->n_items = 0;

    return fc;
}

void append_fc (FACTORIALS *fc, unsigned int n) {
    if (fc->n_items < MAX_FACTORS) {
        fc->factors[fc->n_items] = n;
        fc->n_items++;
    }
    else {
        fprintf(stderr, "append_fc, exceeded max factors %d\n", MAX_FACTORS);
        exit (EXIT_FAILURE);
    }
}

// return last item in factorial array and remove it from factorial
unsigned int extract_fc (FACTORIALS *fc) {

    if (fc->n_items > 0) {
        fc->n_items--;
        return fc->factors[fc->n_items];
    }
    else {
        fprintf(stderr, "Failed to extract item from empty factorial\n");
        exit (EXIT_FAILURE);
    }
}

// recursive function to generate all factorials of a number
void gen_unique_factorials (FACTORIALS *fc, SET *set) {
    unsigned int i, d, num;

    num = extract_fc(fc);   // last element

    for (i = 2; i*i <= num; i++) {
        d = num/i;
        if (d*i == num) {
            append_fc(fc, i);
            append_fc(fc, d);
            if (set_insert(set, fc)) {
                gen_unique_factorials(fc, set);
            }
            else
                extract_fc(fc);

            extract_fc(fc);         // remove d from fc
        }
    }

}

SET *gen_factorials (unsigned int num) {
    unsigned int i, d;
    SET *set;
    FACTORIALS *gfc;

    if ((set = malloc(sizeof(SET))) == NULL) {
        perror("Failed to allocate RAM for set\n");
        exit (EXIT_FAILURE);
    }
    set->next = NULL;

    gfc = init_fc();

    // add to set (1, num) - special case
    gfc->n_items = 2;
    gfc->factors[0] = num;
    gfc->factors[1] = 1;
    set_insert(set, gfc);

    // reset gfc to insclude only num
    gfc->n_items = 1;
    gen_unique_factorials(gfc, set);

    return set;

}

void delete_factorials (SET *sfc) {
    SET *bfc, *nfc;

    if (sfc == NULL)
        return;
    
    nfc = sfc->next;
   
    while (nfc != NULL) {
        bfc = nfc;
        free(bfc->fc);
        nfc = nfc->next;
        free(bfc);
    }

    free(sfc);
}

// compare two factorial set with same amount of factors
int c_factors (FACTORIALS *fc1, FACTORIALS *fc2, int pos) {
    int p = min(fc1->n_items, fc2->n_items);

    if (pos == p)
        return 0;

    if (fc1->factors[pos] == fc2->factors[pos])
        return c_factors(fc1, fc2, pos+1);
    
    if (fc1->factors[pos] > fc2->factors[pos])
        return 1;
    else
        return -1;
}

#ifdef DEBUG
void timer_func (const void *arg) {
    TIMER_S *t_s;

    t_s = (TIMER_S *)arg;
    printf("Timer: current n2: %u, fcs: ", *(t_s->n));
    if (*(t_s->fc) != NULL)
        print_fc(*(t_s->fc));
    else
        printf("\n");
}
#endif

int main (int argc, char **argv) {

    unsigned int n, n2, fc, min_fc, max_repeats;
    mpz_t ln, sqln, sqln2, min_n;
    unsigned *primes, mp;
    bool done = false;
    SET *factorials, *fcs = NULL;
    FACTORIALS *bfc = NULL;
    int n_items, repeats = 0;
    void (*freefunc) (void *, size_t);
    char *str;

#ifdef DEBUG
    TIMER_S t_s = {&bfc, &n2};
    timer_thr tmr;
#endif

    mp_get_memory_functions (NULL, NULL, &freefunc);

    // find closest square of 101*2
    n = atoi(argv[1]);
    n2 = 2*n-1;
    min_fc = 1;
    max_repeats = REPEATS;

    primes = gen_prime(MAX_PRIMES, &mp);

#ifdef DEBUG
    if (setTimer(&tmr, 30, &timer_func, &t_s, true) != 0) {
        fprintf(stderr, "Failed to set timer\n");
        exit (EXIT_FAILURE);
    }
#endif

    mpz_init(sqln);
    mpz_init(sqln2);
    mpz_init(ln);
    mpz_init_set_str(min_n, "99999999999999999999999999999999999999999999999999", 10);

    do {
        n2++;

        bfc = NULL;
        factorials = gen_factorials(n2);

        fcs = factorials->next;
        while (fcs != NULL && !done) {
            // find best fc for a given n_items
            // this limit calculating factors only once per n_items
            n_items = fcs->fc->n_items;
            bfc = fcs->fc;
            while (fcs != NULL && fcs->fc->n_items == n_items) {
                if (c_factors(fcs->fc, bfc, 0) < 0)
                    bfc = fcs->fc;
                fcs = fcs->next;
            }

            calc_factors(bfc, primes, mp, ln);

            mpz_sqrt(sqln, ln);
            mpz_mul(sqln2, sqln, sqln);
            if (mpz_cmp(sqln2, ln) == 0) {

                if (mpz_cmp(min_n, sqln) > 0) {
                    mpz_set(min_n, sqln);
                    min_fc = n2;
                    fc = (n2+1)/2;
                    repeats = 0;
                }
                else
                    repeats++;

#ifdef DEBUG
                str = mpz_get_str(NULL, 10, min_n);
                printf("\nn_items: %u, min_n: %s, fc: %u, min_fc: %u, repeats: %d\n", bfc->n_items, str, n2, min_fc, repeats);
                freefunc(str, strlen(str)+1);
#endif

                if (repeats > max_repeats)
                    done = true;
            }
        }

        delete_factorials(factorials);

    } while (!done);

    str = mpz_get_str(NULL, 10, min_n);
    printf("The least value of n for which the number of distinct solutions exceeds four million, is: %s, distinct solutions: %d\n", str, fc);
    freefunc(str, strlen(str)+1);

    free(primes);
    mpz_clear(ln);
    mpz_clear(sqln2);
    mpz_clear(sqln);
    mpz_clear(min_n);

#ifdef DEBUG
    cancelTimer(&tmr);
#endif

    return 0;
}
