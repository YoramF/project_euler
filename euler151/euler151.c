/**
 * https://projecteuler.net/problem=151
 * 
 * Took me a while to undestand the probability calculations.
 * 
 * Expected number of times (during each week) that the supervisor finds a single sheet of paper in the envelope: 0.464399
 * 
 * real    0m0.003s
 * user    0m0.003s
 * sys     0m0.001s
 */

#include <stdio.h>

typedef union {
    char comb[4] ;           // represent A5, A4, A3, A2 options
    int d;                   // integer (4 bytes) - fast way to check for last sheet
} stage_t;


// main recursive function that generating different combinations
// stage is the current stage
// prob is the probability to get to this stage
double next_comb (stage_t stage, double prob) {
    stage_t l_stage;
    int i, j, c, s;
    double l_prob, comb = 0.0;

    // check if this is a single A5 sheet. if it does, return 0
    if (stage.d == 1)
        return 0.0;

    c = 0;
    for (i = 0; i < 4; i++) {
        c += stage.comb[i];
    }

    // at this point we are shure that the single sheet is not A5 since this case is being caught by checking  if (stage->d == 1)
    if (c == 1) {
        // update probability
        comb += prob;  
    }

    // here come the recursive part
    for (i = 3; i >= 0; i--) {
        l_stage.d = stage.d;

        if (stage.comb[i] > 0) {
            // next stage probability i: current_stage_prob * (number_of_sheets_of type_Ai/total_number_of_sheets)
            l_prob = prob * ((double)stage.comb[i]/c);

            // remove sheet Ai
            l_stage.comb[i]--;

            // and update the number of remaining sheets
            for (j = i; j > 0; j--) {
                // constract new combination and call next_comb
                l_stage.comb[j-1]++;
            }

            // calculate the sum of probabilities down the tree from this stage
            comb += next_comb(l_stage, l_prob);           
        }
    }

    // done
    return comb;
}

int main () {
    double frc;
    stage_t stage;

    stage.d = 0x1010101;    // {A4, A3, A2, A1} -- first stage, probability to get here is 1

    frc = next_comb(stage, 1.0);

    printf("Expected number of times (during each week) that the supervisor finds a single sheet of paper in the envelope: %.6f\n", frc);

    return 0;
}