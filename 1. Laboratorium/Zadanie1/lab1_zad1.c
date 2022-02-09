#include <stdio.h>


unsigned long proper_factors_sum(unsigned long n);


int main() {
    // Get input values
    unsigned long m, n;
    scanf("%lu %lu", &m, &n);

    // Count all perfect numbers in a specified range
    unsigned counter = 0;
    // Store perfect numbers in an array (we assume that there will
    // be no more than 50 perfect numbers within the specified range)
    unsigned long perfect_numbers[50];

    for (unsigned long i = m; i <= n; ++i) {
        // If a number is a perfect number, increment a counter
        if (i == proper_factors_sum(i)) {
            perfect_numbers[counter++] = i;
        }
    }

    // Print results
    printf("%u\n", counter);
    for (size_t i = 0; i < counter; ++i) printf("%lu ", perfect_numbers[i]);

    return 0;
}


unsigned long proper_factors_sum(unsigned long n) {
    if (n <= 1) return 0;

    size_t step = n % 2 + 1; // Skip even values if 'n' is odd
    unsigned long sum = 1;
    for (unsigned long i = 2; i * i <= n; i += step) {
        if (n % i == 0) {
            // If 'i' is a divisor of 'n', also 'n / i' must be its divisor
            sum += i + n / i;
        }
    }
    return sum;
}

