#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>


// generate prime numbers based on sieve of eratosthenes
// support max up to 2^32-1
unsigned *gen_prime_r (unsigned min, unsigned max, unsigned *p_size) {
    unsigned i, k, p, limit, j;
    unsigned *primes = NULL;
    bool *seive;

    min = min < 2? 2: min;

    if (max <= min) {
        fprintf(stderr, "Call to gen_prime_r with invalid arguments max <= min\n");
        return NULL;
    }

    if ((seive = malloc((max+1)*sizeof(bool))) == NULL) {
        fprintf(stderr, "Failed to allocate RAM: %s\n", strerror(errno));
        return NULL;
    }

    // reset seive array to true
    memset(seive, true, max);

    limit = (unsigned)sqrtl((double)max);
    for (i = 2; i <= limit; i++) {
        if (seive[i]) {
            k = i*i;
            j = k;
            p = 1;
            while (j <= max) {
                seive[j] = false;
                j = k+i*p;
                p++;
            }
        }
    }

    // count primes
    k = 0;
    p = 0;
    for (j = 2; j <= max; j++) {
        if (seive[j]) {
            k++;
            if (j < min)
                p++;
        }
    }

    k -= p;

    if (k > 0) {
        if ((primes = (unsigned  *)malloc(k*sizeof(unsigned ))) == NULL) {
            fprintf(stderr, "Failed to allocate RAM: %s\n", strerror(errno));
            return NULL;
        }
    
        j = 0;
        for (i = min; i <= max; i++)
            if (seive[i]) {
                primes[j] = i;
                j++;
            }
    
        *p_size = k;
    }
    else
        *p_size = 0;

    free(seive);
    return primes;       
}

// generate prime numbers based on sieve of eratosthenes
// support max up to 2^64-1
long unsigned  *gen_prime_ll_r (long unsigned min, long unsigned max, long unsigned *p_size) {
    long unsigned i, k, p, limit, j;
    long unsigned *primes = NULL;
    bool *seive;

    min = min < 2? 2: min;
 
    if (max <= min) {
        fprintf(stderr, "Call to gen_prime_r with invalid arguments max <= min\n");
        return NULL;
    }

    if ((seive = malloc((max+1)*sizeof(bool))) == NULL) {
        fprintf(stderr, "Failed to allocate RAM: %s\n", strerror(errno));
        return NULL;
    }

    // reset seive array to true
    memset(seive, true, max);

    limit = (long unsigned)sqrtl((long double)max);
    for (i = 2; i <= limit; i++) {
        if (seive[i]) {
            k = i*i;
            j = k;
            p = 1;
            while (j <= max) {
                seive[j] = false;
                j = k+i*p;
                p++;
            }
        }
    }

    // count primes
    k = 0;
    p = 0;
    for (j = 2; j <= max; j++) {
        if (seive[j]) {
            k++;
            if (j < min)
                p++;
        }
    }

    k -= p;

    if (k > 0) {
        if ((primes = (long unsigned *)malloc(k*sizeof(long unsigned))) == NULL) {
            fprintf(stderr, "Failed to allocate RAM: %s\n", strerror(errno));
            return NULL;
        }
    
        j = 0;
        for (i = min; i <= max; i++)
            if (seive[i]) {
                primes[j] = i;
                j++;
            }
    
        *p_size = k;
    }
    else
        *p_size = 0L;

    free(seive);
    return primes;       
}


// An iterative binary search function.
bool is_prime_r(unsigned x, unsigned arr[], unsigned p_size) {
    long unsigned mid, low = 0, high = p_size-1;

    while (low < high) {
        mid = low + (high - low) / 2;

        // Check if x is present at mid
        if (arr[mid] == x)
            return true;

        // If x greater, ignore left half
        if (arr[mid] < x)
            low = mid + 1;

        // If x is smaller, ignore right half 
        else if (mid > 0)
            high = mid - 1;

        // but don't move beyond 0
        else
            break;
    }

    // we got here if low == high
    // Check if x is present at last position possible (low or high)
    if (arr[low] == x)
        return true;
    else
        return false;
}

// An iterative binary search function.
bool is_prime_ll_r(long unsigned x, long unsigned arr[], long unsigned p_size) {
    long unsigned mid, low = 0, high = p_size-1;

    while (low < high) {
        mid = low + (high - low) / 2;

        // Check if x is present at mid
        if (arr[mid] == x)
            return true;

        // If x greater, ignore left half
        if (arr[mid] < x)
            low = mid + 1;

        // If x is smaller, ignore right half 
        else if (mid > 0)
            high = mid - 1;

        // but don't move beyond 0
        else
            break;
    }

    // we got here if low == high
    // Check if x is present at last position possible (low or high)
    if (arr[low] == x)
        return true;
    else
        return false;
}

// stand alone prime chack
bool is_prime (long unsigned int n) {
    long unsigned i, m;

    if (n != 2 && (n <= 1 || (!(n & 1))))
        return false;

    for (i = 3; i*i <= n; i++) {
        m = n % i;
        if (m == 0L)
            return false;
    }

    return true;
}

