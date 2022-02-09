#include <stdio.h>
#include <limits.h>
#include <stdlib.h>


#define MAX_DIGITS 158

typedef unsigned long long ull;
typedef unsigned short us;

us factorial(us* digits, us* temp, us length, us n);
us multiply(us* digits, us* temp, us length, us prev_begin_idx, ull factor) ;
void print_array(us* digits, us begin_idx, us end_idx);


int main() {
    // Get input value
    us n;
    scanf("%hu", &n);

    // Calculate a factorial and store its digits in an array
    us* digits_ptr = (us*) calloc(MAX_DIGITS, sizeof(us));
    us* temp_ptr = (us*) calloc(MAX_DIGITS, sizeof(us));
    // Store 1 as the last initial factorial digit because factorial
    // will never be lower than 1
    digits_ptr[MAX_DIGITS - 1] = 1;

    // Store the first result digit index
    us first_digit = factorial(digits_ptr, temp_ptr, MAX_DIGITS, n);

    // Print a result
    print_array(digits_ptr, first_digit, MAX_DIGITS);

    // Release memory
    free(digits_ptr);
    free(temp_ptr);

    return 0;
}


us factorial(us* digits, us* temp, us length, us n) {
    ull factor = 1;
    us begin_idx = (us)(length - 1);

    for (us i = 1; i <= n; i++) {
        // If a factor still is small enough to be stored as an unsigned long long
        // number, multiply this factor by the next integer
        if (factor <= ULLONG_MAX / i) factor *= i;
        // On the other hand, we have to stop multiplying this number and store
        // a result of multiplication of a previous value and a current factor
        else {
            begin_idx = multiply(digits, temp, length, begin_idx, factor);
            factor = i; // Begin next time from the next i
        }
    }

    // There will always be some value remaining to place in a digits array, even
    // if a number was never larger than ULLONG_MAX, so we will call multiply
    // function again
    begin_idx = multiply(digits, temp, length, begin_idx, factor);

    // Return an index od the first digit of a factorial calculated
    return begin_idx;
}

us multiply(us* digits, us* temp, us length, us prev_begin_idx, ull factor) {
    // Fill temp array with zeros (only indices used in the previous
    // multiplication)
    for (us i = prev_begin_idx; i < length; i++) temp[i] = 0;

    // Skip zeros stored at the end of a number
    us i = (us)(length - 1);
    while (digits[i] == 0) i--;
    us first_non_zero_idx = i;

    // Store zeros of the current factor
    while (factor % 10 == 0) {
        factor /= 10;
        temp[i] = 0;
        i--;
    }

    // Multiply remaining non-zero numbers and store a result
    // of multiplication in a temporary array
    us result, factor_digit, result_digit, remainder, j, k;
    k = (us)(length - 1);

    while (factor) {
        factor_digit = (us)(factor % 10);
        factor /= 10;
        j = first_non_zero_idx;
        k = i;
        remainder = 0;

        while (j >= prev_begin_idx || remainder != 0) {
            result = (us)(temp[k] + factor_digit * digits[j] + remainder);
            result_digit = (us)(result % 10);
            remainder = result / 10;
            temp[k] = result_digit;
            j--;
            k--;
        }

        i--;
    }

    // Rewrite values from a temporary array to a digits array
    us begin_idx = (us)(k + 1);
    for (i = begin_idx; i <= first_non_zero_idx; i++) digits[i] = temp[i];

    // Return an index of the first result's digit
    return begin_idx;
}

void print_array(us* digits, us begin_idx, us end_idx) {
    for (us i = begin_idx; i < end_idx; i++) printf("%hu", digits[i]);
}
