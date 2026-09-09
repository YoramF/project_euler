/**
 * https://projecteuler.net/problem=155
 * 
 * The way to solve this problem is to by finding sets of fractions S[1]..S[n] where 
 * S[k] = S[i] + S[k-i] (i = 1..k/2)
 * S[1] = {1/1}
 * 
 * S[i] + S[k-1] is calculated as follows:
 * S[i] = {a, b, ...},S[k-i] = {A, B, ...}
 * parallel capacitors: a+A, a+B.. b+A, b+B... 
 * serial capacitors: 1/(1/a + 1/A), 1/(1/a + 1/B)... 1/(1/b + 1/A), 1/(1/b + 1/B)...
 * No duplicate valuse within set S[k].
 * 
 * Once S[1]..S[18] are calculated we need to merge all sets into  one -> D[18] and count how many values we have and
 * that is the answer.
 * 
 * I used sets library to handle duplicate values.
 * 
 * 
 * D(18) = 3857447
 * 
 * real    0m25.508s
 * user    0m25.298s
 * sys     0m0.203s
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sets.h>

#define MAX             19
#define ALLOC_SIZE      10000
#define SET_HASH_SIZE   1000000

typedef struct {
    unsigned int num;        // Numerator
    unsigned int den;        // Denominator
} fraction_t;

typedef struct fraction_set {
    int          free;      // free space to write new fraction
    int          blocks;    // number of allocated blocks
    unsigned int count;     // number of fractions in set
    fraction_t *fractions;  // array of reactions
} fraction_set_t;

// add new fraction to fraction set
void s_add (fraction_set_t *s, fraction_t f) {

    if ((s->fractions == NULL)) {
        if ((s->fractions = malloc(ALLOC_SIZE * sizeof(fraction_t))) == NULL) {
            perror("Failed to allocate RAM for new fraction");
            exit(EXIT_FAILURE);
        }
        s->fractions->num = f.num;
        s->fractions->den = f.den;
        s->count = 0;
        s->free = 0;
        s->blocks = 1;
    }
    else {
        if (s->free == 0) {
            (s->blocks)++;
            if ((s->fractions = realloc(s->fractions, s->blocks * ALLOC_SIZE * sizeof(fraction_t))) == NULL) {
                perror("Failed to reallocate RAM for new fraction");
                exit(EXIT_FAILURE);
            }
            s->free = ALLOC_SIZE;
        }

        s->fractions[s->count].num = f.num;
        s->fractions[s->count].den = f.den;
        (s->free)--;
    }
    (s->count)++;
}

// find commong denominator of a, b
unsigned int gdc (unsigned int a, unsigned int b) {
    unsigned int x;

    x = a % b;
    while (x != 0) {
        a = b;
        b = x;
        x = a % b;
    }

    return b;
}

// add two fraction f = f1+f2
fraction_t f_add (fraction_t f1, fraction_t f2) {
    fraction_t f;
    unsigned int g;

    if (f1.den == f2.den) {
        f.num = f1.num + f2.num;
        f.den = f1.den;
    }
    else {
        f.num = (f2.den * f1.num) + (f1.den * f2.num);
        f.den = f1.den * f2.den;
    }

    g = gdc(f.num, f.den);
    if (g > 1) {
        f.num /= g;
        f.den /= g;
    }

    return f;
}

// inverse a fraction f = 1/f1
fraction_t f_inv (fraction_t f1) {
    fraction_t f;

    f.num = f1.den;
    f.den = f1.num;

    return f;
}

int main () {
    fraction_set_t f_sets[MAX] = {0, 0, 0, NULL};
    SET *set;
    unsigned int sum;
    fraction_t f1, f2, f1_inv, f2_inv, f;
    int s;

    // init set
    set = set_create(SET_HASH_SIZE, sizeof(fraction_t));
    sum = 0;

    s_add(&f_sets[1], (fraction_t){1,1});

    for (int k = 2; k < MAX; k++) {

        // init set
        set_clear(set);
        
        for (int i = 1; i <= k / 2; i++) {
            for (int a = 0; a < f_sets[i].count; a++) {
                f1 = f_sets[i].fractions[a];

                for (int b = 0; b < f_sets[k - i].count; b++) {
                    f2 = f_sets[k - i].fractions[b];

                    // parallel capacitors
                    f = f_add(f1, f2);
                    if ((s = set_insert(set, &f)) == 1) {
                        s_add(&f_sets[k], f);
                    }
                    else if (s < 0) {
                        perror("Failed to insert into set");
                        exit(EXIT_FAILURE);
                    }

                    // serial capacitors
                    f1_inv = f_inv(f1);
                    f2_inv = f_inv(f2);
                    f = f_add(f1_inv, f2_inv);
                    f = f_inv(f);
                    if ((s = set_insert(set, &f)) == 1) {
                        s_add(&f_sets[k], f);
                    }
                    else if (s < 0) {
                        perror("Failed to insert into set");
                        exit(EXIT_FAILURE);
                    }           
                }
            }

        }
    }

    // sum all capacitance values
    // avoid duplications
    // generate final super set. on the way count number of elements in final super set.
    set_clear(set);
    for (int k = 1; k < MAX; k++) {
        for (int i = 0; i < f_sets[k].count; i++) {
            if ((s = set_insert(set, &f_sets[k].fractions[i])) == 1)
                sum++;
        }
    }

    printf("D(18) = %u\n", sum);

    return 0;
}