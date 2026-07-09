/**
 * https://projecteuler.net/problem=149
 * 
 * First write a function that scan a square (n*n) and find the maximum possible sum of adjacent numbers in any direction
 * Then funtion that gets a row of integers and find the Maximum-sum Subsequence
 * Last a function that scan the created matrix in all directions and find the greatest sum.
 * 
 * Solution:
 * greatest sum: 52852124
 * 
 * real    0m0.092s
 * user    0m0.087s
 * sys     0m0.006s
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MATRIX_DIM 2000

/**
 * Find the Maximum-sum Subsequence of a given row of integers
 * The idea is the keep two max sums
 * 1) sum of subsequance positive integers
 * 2) sum of subsequance integers starting from best position in row
 * Since we are moving only forward it is guaranty that c_max is compose of Subsequence integers
 */
int mamximum_sum_subsequence (int *row, int len) {
    int c_max = 0x80000000;     // current best max sum of only positive integers
    int b_sum = 0x80000000;     // sum of integers starting from best possition
    for (int i = 0; i < len; i++) {
        if (b_sum < 0 && row[i] > b_sum)
            b_sum = row[i];
        else
            b_sum += row[i];

        if (c_max < b_sum)
            c_max = b_sum;
    }

    return c_max;
}

/**
 * scan the matrix horizontal, vertical, diagonal or anti-diagonal
 */
int f_max (int *matrix, int *row1, int *row2, int d) {
    int max = 0x80000000;
    int i, j, s, r, jc, sc;

    // scan all horizontal rows
    for (i = 0; i < d; i++) {
        r = i * d;
        for (j = 0; j < d; j++)
            row1[j]= matrix[r + j];
        
        s = mamximum_sum_subsequence(row1, d);
        if (s > max)
            max = s;

    }

    // scan all vertical columns
    for (j = 0; j < d; j++) {
        r = 0;
        for (i = 0; i < d; i++) {
            row1[i] = matrix[r + j];
            r += d;
        }

        s = mamximum_sum_subsequence(row1, d);        
        if (s > max)
            max = s;
    }


    // scan top left to right side diagonals
    // here we calso sum the cross directions 
    for (i = 0; i < d - 1; i++) {
        r = i * d;
        for (j = 0; j < d - i; j++) {
            jc = d - j - 1;
            row1[j] = matrix[r + j];
            row2[j] = matrix[r + jc];
            r += d;
        }
        s = mamximum_sum_subsequence(row1, d - i);
        if (s > max)
            max = s;
        s = mamximum_sum_subsequence(row2, d - i);
        if (s > max)
            max = s;
    }


    // scan top left downward side diagonals
    for (i = 0; i < d - 2; i++) {
        int k = 0;
        r = 0;
        for (j = i + 1; j < d; j++) {
            jc = d - j - 1;
            row1[k] = matrix[r + j];
            row2[k] = matrix[r + jc];
            r += d;
            k++;
        }
        s = mamximum_sum_subsequence(row1, k);
        if (s > max)
            max = s;
        s = mamximum_sum_subsequence(row2, k);
        if (s > max)
            max = s;
    }

    return max;
}


/**
 * Generate the base matrix according to "Lagged Fibonacci Generator"
 */
int *gen_matrix (int dim) {
    int *matrix;
    int k;
    long int d;

    if ((matrix = malloc(dim*dim*sizeof(int))) == NULL) {
        perror("Failed to allocate RAM");
        exit(EXIT_FAILURE);
    }

    // generate the first 55 elements
    for (k = 1L; k <= 55L; k++) {
        d = (100003L - (200003L * k) + (300007L * k * k * k)) % 1000000L - 500000L;
        matrix[k-1] = (int)d;
    }

    // generate the rest of the numbers
    for (k = 56L; k <= 4000000L; k++) {
        d = (matrix[k-25] + matrix[k-56] + 1000000L) % 1000000L - 500000L;
        matrix[k-1] = (int)d;
    }

    return matrix;
}

int main () {
    int max;
    int *matrix;
    int *row1, *row2;

    if ((row1 = malloc(MATRIX_DIM*sizeof(int))) == NULL) {
            perror("Failed to allocate RAM");
            exit(EXIT_FAILURE);
    }

    if ((row2 = malloc(MATRIX_DIM*sizeof(int))) == NULL) {
            perror("Failed to allocate RAM");
            exit(EXIT_FAILURE);            
    }

    matrix = gen_matrix(MATRIX_DIM);
    max = f_max(matrix, row1, row2, MATRIX_DIM);

    printf("greatest sum: %d\n", max);

    free(matrix);
    free(row2);
    free(row1);

    return 0;
}