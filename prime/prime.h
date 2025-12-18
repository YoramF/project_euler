#ifndef PRIME_H_
#define PRIME_H_

#include <stdbool.h>

/*
    When calling prime functions you must include gmp (-lgmp) amd math (-lm) in your
    gcc link command 
*/

unsigned *gen_prime_r (unsigned min, unsigned max, unsigned *p_size);
long unsigned *gen_prime_ll_r (long unsigned min, long unsigned max, long unsigned *p_size);
bool is_prime_r(unsigned x, unsigned arr[], unsigned p_size);
bool is_prime_ll_r(long unsigned x, long unsigned arr[], long unsigned p_size);
bool is_prime(long unsigned int n);

#define gen_prime(m,p) gen_prime_r(0,(m),(p))
#define gen_prime_ll(m,p) gen_prime_ll_r(0,(m),(p))

#endif /* PRIME_H_ */