/**
 * A comlext numbers arithmetic functions
 */
#include <stdio.h>
#include <imaginary.h>

/**
 * Add two complex numbers c1 & c2 and return complex result
 */
complex_t c_add (complex_t c1, complex_t c2) {
    complex_t r;

    r.re = c1.re + c2.re;
    r.im = c1.im + c2.im;

    return r;
}

/**
 * Multiply complex number c1 by complex number c2 and return new complex result
 */
complex_t c_mul (complex_t c1, complex_t c2) {
    complex_t r;
    // im_t im;
    long int im;
    long int re;

    re = c1.re * c2.re;

    im = (c1.re * c2.im) + (c2.re * c1.im);
    
    re += -(c1.im * c2.im);

    r.im = im;
    r.re = re;

    return r;
}

/**
 * Raise complex number c1 by power pwr and return complex result
 */
complex_t c_pwr (complex_t c1, int pwr) {
    complex_t r;

    // init r to 1 == (1 + 0i)
    r.re = 1L;
    r.im = 0L;

    for (int i = 0; i < pwr; i++)
        r = c_mul(r, c1);

    return r;
}

char  *c_print (complex_t c, char *s) {
    long int im;
    char op;

    im = c.im;
    if (im >= 0L)
        op = '+';
    else {
        im = -im;
        op = '-';
    }
    sprintf(s, "(%ld %c %ldi)", c.re, op, im);
    return s;
}


// int main () {
//     // im_t im, imr;
//     complex_t c;
//     char c_str [100];
//     int pwr;

//     while (1) {
//         printf("re,im ,pwr: ");
//         scanf("%d, %d, %d", &c.re, &c.im, &pwr);

//         printf("input: complex: %s, pwr: %d = ", c_print(c, c_str), pwr);
//         c = c_pwr(c, pwr);
//         printf("%s\n", c_print(c, c_str));
//     }
// }