/**
 * https://projecteuler.net/problem=150
 * 
 * To calculate the position of the first element in the tirangle in line n we use the arithmetic sum formula, were
 * the first line is 0, d == 1, and the first element (a1 == 1)
 * The index of an = n/2 * (2*a1 + (n-1)*d) ==> n/2 * (2 + (n-1)).
 * In line n+1 there are n+1 elements.
 * 
 * Brute Force solution:
 * min_sub_triange_val: -271248680
 * 
 * real    1m51.135s
 * user    1m51.115s
 * sys     0m0.006s
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM         500500
#define LAST_ROW        1000
#define TWO_POWR_20     1048576
#define TWO_POWER_19    524288
#define MAX_VAL         0x7FFFFFFFFFFFFFFF

// calculate the index of element in the triangle array based on row and position within the row
// (n*(n+1)) is always even!! so t_id can't be a fraction number.
#define t_id(r,p)   ((((r)*(1+(r)))/2)+(p))

/**
 * Generate the initial triangle
 */
long int init_triangle (int *tr) {
    long int t;
    long int  s = 0;

    t = 0L;
    for (int i = 0; i < MAX_NUM; i++) {
        t = (615949 * t + 797807) % TWO_POWR_20;
        tr[i] = (int)(t - TWO_POWER_19);
        s += tr[i];
    }

    return s;
}

/**
 * return the sum of a row (r) of length (l)
 */
long int sum_r (int *r, int l) {
    long int s = 0L;

    for (int i = 0; i < l; i++)
        s += (long int)r[i];

    return s;
}

/**
 * Given a starting element (row, pos), return the smallest value of a possible downward triangle
 * 
 */
long int min_val (int r, int p, int *tr) {
    long int v = MAX_VAL;
    long int s = 0L;
    int l = 1;

    while (r < LAST_ROW) {
        s += sum_r(&tr[t_id(r, p)], l);
        l++;
        if (s < v) {
            v = s;
        }
        r++;
    }

    return v;
}

int main () {
    int *tr ;
    long int ts;
    long int m_val = MAX_VAL;

    if ((tr = malloc(MAX_NUM * sizeof(int))) == NULL) {
        perror("Failed to allocate RAM");
        exit(EXIT_FAILURE);
    }

    ts = init_triangle(tr);
    printf("Init triangle sun: %ld\n", ts);


    // scan the triangle
    for (int r = 0; r < LAST_ROW; r++) {
        for (int p = 0; p < r + 1; p++) {
            ts = min_val(r, p, tr);

            if (m_val > ts) {
                m_val = ts;
            }
        }
    }

    printf("min_sub_triange_val: %ld\n", m_val);

    free(tr);

    return 0;
}