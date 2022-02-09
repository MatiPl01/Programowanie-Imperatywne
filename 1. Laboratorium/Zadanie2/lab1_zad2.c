#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


bool has_non_decreasing_digits(unsigned long n);
void sieve_of_eratosthenes(unsigned long n, bool compound[]);


int main() {
    // Get input data
    unsigned long n;
    scanf("%lu", &n);

    // Fill an array with boolean values (true if not prime)
    bool *compound = calloc(n, sizeof(compound));
    sieve_of_eratosthenes(n, compound);

    // Loop over 'compound' array and print values that have
    // digits of a non-decreasing order
    for (unsigned long val = 2; val < n; val++) {
        // If a value is not compound (is a prime number),
        // check if it has digits of a non-decreasing order
        if (!compound[val] && has_non_decreasing_digits(val)) {
            printf("%lu\n", val);
        }
    }

    return 0;
}

bool has_non_decreasing_digits(unsigned long n) {
    if (n < 10) return true;

    size_t prev_digit = n % 10;
    n /= 10;

    // Loop from a back side of a number 'n' and check if subsequent
    // numbers are increasing (if they fo so, return false)
    while (n > 0) {
        size_t curr_digit = n % 10;
        n /= 10;
        if (curr_digit > prev_digit) return false;
        prev_digit = curr_digit;
    }

    return true;
}


// Generates primes list from 2 to 'n'
void sieve_of_eratosthenes(unsigned long n, bool compound[]) {
    // Cross out values that are compound (not primes)
    for (unsigned long val = 2; val < n; val++) {
        if (!(compound[val])) {
            for (unsigned long i = val * val; i < n; i += val) {
                compound[i] = true;
            }
        }
    }
}
