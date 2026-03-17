/*
https://projecteuler.net/problem=142

First attempt: brute force. Work with pairs. First (x,z) where x is from 3 to max (Guess too small and you will not
find the answer). Then once we find firt pair, find first pair (y,z) where  z < y < x. Once this pair was found, check
if also (x,y) is perfect square. If it does, we found the answer.

Asnswer:
x: 434657, y: 420968. z:150568
x+y+z = 1006193

real    4m55.740s
user    4m55.675s
sys     0m0.013s

Running in parallel:
real    0m37.170s
user    6m11.608s
sys     0m0.011s

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <async.h>

#define MAX_NUM 10000003
#define SEG 10000

typedef struct {
    unsigned int x, y, z;
} func_c_arg_t;

typedef struct {
    unsigned int x, max;
} arg_t;

static int s_complete = 0;
static async_s_func_t s_f;

void func (void *arg, void *res) {
    unsigned int x = (*(arg_t *)arg).x;
    unsigned int max = (*(arg_t *)arg).max;
    unsigned int y, z, d, sq;
    func_c_arg_t r;

    free(arg);

    for (; x < max && s_complete == 0; x++) {
        for (z = 1; z < x && s_complete == 0; z++) {
            d = x-z;
            sq = (unsigned int)sqrt((double)d);
            if (sq*sq != d)
                continue;
            d = x+z;
            sq = (unsigned int)sqrt((double)d);
            if (sq*sq == d) {
                for (y = z+1; y < x && s_complete == 0; y++) {
                    d = y-z;
                    sq = (unsigned int)sqrt((double)d);
                    if (sq*sq != d)
                        continue;
                    d = y+z;
                    sq = (unsigned int)sqrt((double)d);
                    if (sq*sq == d) {
                        d = x+y;
                        sq = (unsigned int)sqrt((double)d);
                        if (sq*sq != d)
                            continue;
                        d = x-y;
                        sq = (unsigned int)sqrt((double)d);
                        if (sq*sq == d) {
                            printf("x: %u, y: %u. z:%u\n", x, y, z);
                            r.x = x;
                            r.y = y;
                            r.z = z;
                            async_call(s_f, &r, NULL);
                            return;
                        }       
                    }               
                }
            }                           
        }
    }
}

void func_s (void *arg, void *res) {
    func_c_arg_t *a = (func_c_arg_t *)arg;
    s_complete = 1;      
    printf("sum: %u\n", a->x + a->y + a->z);   
}

int main () {
    unsigned int x;
    int i;
    async_id_t  async_id;

    async_id = async_init (10);
    s_f = async_sync_func_init(func_s, async_id);

    x = 3;
    while (x < MAX_NUM && s_complete == 0) {
        for (i = 0; i < 10 && i*SEG < MAX_NUM; i++) {
            arg_t *arg = malloc(sizeof(arg_t));
            arg->x = x;
            x += SEG;
            arg->max = x;
            async_launch(func, NULL, arg, NULL, NULL, async_id);
        }
    }

    async_terminate(async_id);

    return 0;
}