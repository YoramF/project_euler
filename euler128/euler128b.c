/*
https://projecteuler.net/problem=128


If N is the number of tiles in ring n, then ring n+1 will have N+6 tiles and so on.So we have a

So we have an Arithmetic Series 6, 12, 18, 25... An = n*6.
First tile in ring 1 is 2, so first tile in ring 2 is 2+1*6=8...
and total number of tiles including the first single one is: 3N*(N+1) where N is the number of rings.
After running some iterations with different number of ring, it was clear the the only Tiles which have
3 primes with their neighbors tiles values are the first or the last tiles in a ring. 

Tile: 14516824220, pdc: 2000

real    6m34.734s
user    6m34.357s
sys     0m0.134s

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include <prime.h>

#define MAX_RINGS  10000
#define ALLOC_TILES  (MAX_RINGS*6)
#define TARGET  2000

#define SINGLE 0
#define DOUBLE 1
#define n_tiles(r) ((r)*6)               // calculate number of tiles per ring
#define l_tile(t, r) ((t)+(r)*6)         // calculate the last tile number in ring given the first tile number in that ring
#define p_tile(t, r) ((t)-((r)-1)*6)     // calculate the first tile number of privious ring given the first tile number of current ring
#define opposite_a(n) (((n)+3)%6)        // calculate the opposite adge
#define n_adge(n) (((n)+1)%6)
#define p_adge(n) (((n)+5)%6)
#define b_adge(n) ((n)%6)
#define delta(a, b) ((a)>(b)?(a)-(b):(b)-(a))                   


typedef struct {
    long int     val;            // Tile value
    long int     neighbors [6];  // 6 neighbors numbers, [0] is bottom and counting counter clockwise
    int     type;           // number of sides to lower ring (SINGLE, DOUBLE)
    int     ring;           // ring number

} TILE;

// static unsigned int *Primes, Ps;
static TILE *Tiles[2];

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
    unsigned int i, ring;
    unsigned int adge, type, c_count, l_count;
    char adges[1000];
    int pdc = 0, pd;
    int allocated_tiles;
    int l_ring, c_ring;
    long int current, last, first, o, t_tile;

    // allocate Tiles
    if ((Tiles[0] = malloc(ALLOC_TILES*sizeof(TILE))) != NULL) {
        if ((Tiles[1] = malloc(ALLOC_TILES*sizeof(TILE))) == NULL) {
            perror("Failed to allocate RAM for Tiles");
            free(Tiles[0]);
            exit(EXIT_FAILURE);            
        }
    }
    else {
        perror("Failed to allocate RAM for Tiles");
        exit(EXIT_FAILURE);
    }
    allocated_tiles = ALLOC_TILES;

    // fill Tiles array;
    l_ring = 0;
    c_ring = 1;
    Tiles[l_ring][0].ring = 0;
    Tiles[l_ring][0].type = 0;
    Tiles[l_ring][0].val = 1L;
    memset(Tiles[l_ring][0].neighbors, 0, sizeof(Tiles[l_ring][0].neighbors));
    current = 2L;

    ring = 1;
    while (pdc < TARGET) {
        type = 0;

        last = l_tile(current, ring)-1;
        l_count = 0;
        first = current;
    
        // start with lower adge
        adge = 0;
        for (c_count = 0; c_count < n_tiles(ring); c_count++) {
            // reset all adges
            memset(Tiles[c_ring][c_count].neighbors, 0, sizeof(Tiles[c_ring][c_count].neighbors));
            Tiles[c_ring][c_count].ring = ring;
            Tiles[c_ring][c_count].val = current;
            Tiles[c_ring][c_count].type = type % ring == 0? SINGLE: DOUBLE;
            if ((Tiles[c_ring][c_count].type == SINGLE)) {
                // set next adge
                Tiles[c_ring][c_count].neighbors[p_adge(adge)] = current < last? current+1: first;
                // set lower adge & update lower tile opposite adge
                Tiles[c_ring][c_count].neighbors[b_adge(adge)] = Tiles[l_ring][l_count].val;
                Tiles[l_ring][l_count].neighbors[opposite_a(adge)] = current;
                // set previous adge
                Tiles[c_ring][c_count].neighbors[n_adge(adge)] = current == first? last: current-1;
                adge++;
            }
            else {
                // set next adge
                Tiles[c_ring][c_count].neighbors[p_adge(adge-1)] = current < last? current+1: first;
                // set two lower adge & their lower tiles opposite adges
                Tiles[c_ring][c_count].neighbors[b_adge(adge)] = Tiles[l_ring][l_count].val;
                Tiles[l_ring][l_count].neighbors[opposite_a(adge)] = current;
                l_count++;
                o = current < last? l_count: 0;
                Tiles[c_ring][c_count].neighbors[p_adge(adge)] = Tiles[l_ring][o].val;
                Tiles[l_ring][o].neighbors[opposite_a(p_adge(adge))] = current;
                // set previous adge
                Tiles[c_ring][c_count].neighbors[n_adge(adge)] = current == first? last: current-1;                
            }
   
            current++;
            type++;
        }

        // calculate PD(n)
        // for (i = 0; i < (l_count? l_count: 1); i++) {
        //     if ((pd = PD(Tiles[l_ring][i].val, Tiles[l_ring][i].neighbors)) == 3) {
        //         pdc++;
        //         printf("%d:%ld %d\r", Tiles[l_ring][i].ring, Tiles[l_ring][i].val, pdc);
        //     }

        //     // printf("[%d.%d.%d] %d, %s::%d\n",Tiles[l_ring][i].ring, Tiles[l_ring][i].val, i, Tiles[l_ring][i].type, p_adges(Tiles[l_ring][i].neighbors, adges), pd);
        // }
        if ((pd = PD(Tiles[l_ring][0].val, Tiles[l_ring][0].neighbors)) == 3) {
            pdc++;
            t_tile = Tiles[l_ring][0].val;
            printf("%d:%ld %d\r", Tiles[l_ring][0].ring, Tiles[l_ring][0].val, pdc);        
        }
        if (l_count && ((pd = PD(Tiles[l_ring][l_count-1].val, Tiles[l_ring][l_count-1].neighbors)) == 3)) {
            pdc++;
            t_tile = Tiles[l_ring][l_count-1].val;
            printf("%d:%ld %d\r", Tiles[l_ring][l_count-1].ring, Tiles[l_ring][l_count-1].val, pdc);
        }

        // check if we need to allocate more RAM
        if (c_count == allocated_tiles) {
            allocated_tiles += ALLOC_TILES;
            if ((Tiles[0] = realloc(Tiles[0], allocated_tiles*sizeof(TILE))) != NULL) {
                if ((Tiles[1] = realloc(Tiles[1], allocated_tiles*sizeof(TILE))) == NULL) {
                    perror("Failed to allocate RAM for Tiles");
                    exit(EXIT_FAILURE);            
                }
            }
            else {
                perror("Failed to allocate RAM for Tiles");
                exit(EXIT_FAILURE);
            }
        }

        // switch rings
        l_ring = c_ring;
        c_ring = 1-l_ring;
        ring++;
    }
 
    printf("\nTile: %ld, pdc: %d\n", t_tile, pdc);
    free(Tiles[0]);
    free(Tiles[1]);

    return 0;

} 