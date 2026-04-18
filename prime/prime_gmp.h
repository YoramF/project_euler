#ifndef PRIME_GMP_H_
#define PRIME_GMP_H_

#include <stdbool.h>
#include <gmp.h>

/*
    When calling prime functions you must include gmp (-lgmp) amd math (-lm) in your
    gcc link command 
*/

bool is_prime_gmp(mpz_t n);
bool is_prime_g (long unsigned int n);

#endif /* PRIME_GMP_H_ */