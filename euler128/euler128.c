/*
https://projecteuler.net/problem=128


If N is the number of tiles in ring n, then ring n+1 will have N+6 tiles and so on.So we have a

So we have an Arithmetic Series 2, 8, 20, 38 ... An = An-1+6*n = 2+3*n*(n+1).
To follow this problem we will consider first ring as 8-19, and ring 2-7 as ring 0.

After running some iterations with different number of ring, it was clear the the only Tiles which have
3 primes with their neighbors tiles values are the first or the last tiles in a ring.
Next level optimization is to work only on the first and last Tiles in a ring.
These are the rules of neughbors counting from bottom counter clockwize:
FIrst tile in a ring:
0: First tile in previous ring
1: Last tile in current ring
2: Last tile in next ring
3: First in next ring
4: Second in next ring
5: Next in current ring

Last tile in a ring:
0: Last in previus ring
1: Previous in current ring
2: (Last in next ring)-1
3: Last in next ring
4: First in current ring
5: First in previous ring

Since we know that inner Tile has 3 Primes difference, we start with ring 1 where A1 = 2

Tile: 14516824220, pdc: 2000

real    0m0.637s
user    0m0.630s
sys     0m0.007s

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include <prime.h>

#define TARGET  2000

#define SINGLE 0
#define DOUBLE 1

#define last_n(r)  (2+3*(long int)((r)+2)*((r)+3)-1)
#define first_c(r) (2+3*(long int)(r)*((r)+1))
#define first_n(r) (2+3*(long int)((r)+1)*((r)+2))
#define first_p(r) (2+3*(long int)(r)*((r)-1))


#define delta(a, b) ((a)>(b)?(a)-(b):(b)-(a))                   


// unsigned int *Primes, Ps;

int PD (long int current, long int neighbors[]) {
    long int d;
    int i, p;

    p = 0;
    for (i = 0; i < 6; i++) {
        d = delta(neighbors[i],current);
        // if (is_prime_r((unsigned int)d, Primes, Ps))
        if (is_prime(d))
            p++;
    }   
    return p;
}

char *p_adges (long int a[], char *s) {
    sprintf(s, "%ld %ld %ld %ld %ld %ld", a[0], a[1], a[2], a[3], a[4], a[5]);
    return s;
}


int main () {
    unsigned int ring;
    char adges[1000];
    int pdc, pd;
    long int t_tile;
    long int neighbors[6];
    long int f_c, f_n, f_p, l_n;

    ring = 1;
    pdc = 2;    // Tiles 1 & 2 has 3 primes
    while (pdc < TARGET) {

        l_n = last_n(ring);
        f_c = first_c(ring);
        f_p = first_p(ring);
        f_n = first_n(ring);
 
        // check First tile in ring
        neighbors[0] = f_p;
        neighbors[1] = f_n-1;
        neighbors[2] = l_n;
        neighbors[3] = f_n;
        neighbors[4] = f_n+1;
        neighbors[5] = f_c+1;

        if ((pd = PD(f_c, neighbors)) == 3) {
            pdc++;
            t_tile = f_c;
            // printf("[%d] %ld, %d\r", ring, f_c, pdc);
        }

        // check Last tile in ring
        neighbors[0] = f_p-1;
        neighbors[1] = f_c-1;
        neighbors[2] = l_n-1;
        neighbors[3] = l_n;
        neighbors[4] = f_c;
        neighbors[5] = f_p;

        if ((pd = PD(f_n-1, neighbors)) == 3) {
            pdc++;
            t_tile = f_n-1;
            // printf("[%d] %ld, %d\r", ring, f_n-1, pdc);
        }

        ring++;
    }
 
    printf("\nTile: %ld, pdc: %d\n", t_tile, pdc);

    return 0;

} 