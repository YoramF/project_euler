/**
 * A comlext numbers arithmetic functions
 */

#include <stdio.h>
#include <stdbool.h>

#define set_im(im,val)  ({\
        (im).num  = (val);\
        (im).is_im = true;})

typedef struct {
    int     num;
    bool    is_im;
} im_t;

typedef struct {
    int         re;     // real p[art
    im_t        im;     // imaginary part
} complex_t;


im_t i_pwr (im_t im, int pwr) {
    im_t im_r;
    int s = 1;

    switch (pwr % 4) {
        case 0:
            im_r.is_im = false;
            break;
        case 1:
            im_r.is_im = im.is_im;
            break;
        case 2:
            s = -1;
            im_r.is_im = false;
            break;
        case 3:
            s = -1;
            im_r.is_im = im.is_im;            

    }

    im_r.num = 1;
    for (int j = 0; j < pwr; j++)
        im_r.num *= im.num;

    im_r.num *= s;

    return im_r;   
}

int main () {
    im_t im, imr;
    int num;
    int pwr;

    while (1) {
        printf("im: ,pwr: ");
        scanf("%d, %d", &num, &pwr);
        set_im(im, num);
        im.is_im = true;

        imr = i_pwr(im, pwr);
        printf("im.num=%d, im.i=%d\n", imr.num, imr.is_im);
    }
}