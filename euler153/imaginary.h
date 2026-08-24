#ifndef IMAGINARY_H_
#define IMAGINARY_H_

typedef struct {
    long int         re;     // real part
    long int         im;     // imaginary part
} complex_t;

#define set_comp(comp, r, i)            ({(comp).re=(long int)(r);(comp).im=(long int)(i);})
#define set_comp_from_int(comp, r)      ({(comp).re=(long int)(r);(comp).im=0L;})
#define set_comp_div_from_2(comp)       ({(comp).re=1L;(comp).im=1L;})

char  *c_print (complex_t c, char *s);
complex_t c_add (complex_t c1, complex_t c2);
complex_t c_mul (complex_t c1, complex_t c2);
complex_t c_pwr (complex_t c1, int pwr);

#endif /* IMAGINARY_H_ */