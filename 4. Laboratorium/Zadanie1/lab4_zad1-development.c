#include <stdio.h>
#include <limits.h>
#include <stdlib.h>


#define MAX_DIGITS 158

typedef unsigned long long ull;
typedef unsigned short us;

us factorial(us* digits, us* temp, us length, us n) ;
us multiply(us* digits, us* temp, us length, us prev_begin_idx, ull factor) ;
void print_array(us* digits, us begin_idx, us end_idx);


int main() {
//    ull a = ULLONG_MAX;
//    short digit;
//    short arr[100];
//    short i = 99;
//    while(a) {
//        digit = (short)(a % 10);
//        a /= 10;
//        arr[i] = digit;
//        i--;
//    }
//    i++;
//    for (short j = i; j < 100; j++) printf("%d", arr[j]);

    // Get input value
    us n;
    scanf("%hu", &n);

    // Calculate a factorial and store its digits in an array
    us* digits_ptr = (us*) calloc(MAX_DIGITS, sizeof(us));
    us* temp_ptr = (us*) calloc(MAX_DIGITS, sizeof(us));
    digits_ptr[MAX_DIGITS - 1] = 1;

    print_array(digits_ptr, 0, MAX_DIGITS);

    us first_digit = factorial(digits_ptr, temp_ptr, MAX_DIGITS, n);

//    for (us i = 0; i < MAX_DIGITS; i++) digits_ptr[i] = 0;

    // Print a result
    print_array(digits_ptr, first_digit, MAX_DIGITS);

    printf("\n\n\n%hd", digits_ptr[0]);    // Release memory
    free(digits_ptr);
    free(temp_ptr);

    return 0;
}


us factorial(us* digits, us* temp, us length, us n) {
    ull factor = 1;
    us begin_idx = (us)(length - 1);
    for (us i = 1; i <= n; i++) {
        if (factor <= ULLONG_MAX / i) {
            factor *= i;
            ull val = factor;
            us digit;
            us arr[100];
            short j = 99;
            while(val) {
                digit = (us)(val % 10);
                val /= 10;
                arr[j] = digit;
                j--;
            }
            j++;
            for (short k = j; k < 100; k++) printf("%d", arr[k]);
            printf("\n");
        }
        else {
            printf("\n\n===== Multiplying prev result: =====\n\n");
            begin_idx = multiply(digits, temp, length, begin_idx, factor);
            factor = i;
        }
    }

    printf("\n\n===== Multiplying prev result: =====\n\n");
    begin_idx = multiply(digits, temp, length, begin_idx, factor);
    printf("\n\nBegin idx: %hd\n", begin_idx);

    return begin_idx;
}

us multiply(us* digits, us* temp, us length, us prev_begin_idx, ull factor) {
    for (us x=0; x<length;x++) temp[x]=0;

    printf("\nNew factor: ");
    ull val = factor;
    us digit;
    us arr[100];
    short x = 99;
    while(val) {
        digit = (us)(val % 10);
        val /= 10;
        arr[x] = digit;
        x--;
    }
    x++;
    for (short q = x; q < 100; q++) printf("%d", arr[q]);
    printf("\n");

    us first_non_zero_idx, i;
    i = (us)(length - 1);
    printf("\nBegin digit %hd", i);
    // Skip zeros stored at the end of a number
    while (digits[i] == 0) i--;
    first_non_zero_idx = i;

    printf("\nfirst non zero %hd", first_non_zero_idx);

    // Store zeros of the current factor
    while (factor % 10 == 0) {
        factor /= 10;
        temp[i] = 0;
        i--;
    }

    printf("\ni after adding zeros %hd", i);

    // Multiply remaining non-zero numbers and store a result
    // of multiplication in a temporary array
    us result, factor_digit, result_digit, remainder, j, k;
    k = (us)(length - 1);
    while (factor) {
        factor_digit = (us)(factor % 10);
        printf("\n\nFactor digit %hd", factor_digit);
        factor /= 10;
        j = first_non_zero_idx;
        k = i;
        remainder = 0;

        while (j >= prev_begin_idx || remainder != 0) {
            printf("\nMultiplying %hd * %hd", factor_digit, digits[j]);
            result = (us)(temp[k] + factor_digit * digits[j] + remainder);
            result_digit = (us)(result % 10);
            remainder = result / 10;
            printf("\nresult %hd, result_digit %hd, remainder %hd", result, result_digit, remainder);
            printf("\nCurrent temp[%hd]: %hd", k, temp[k]);
            temp[k] = result_digit;
            printf("\nStoring result: temp[%hd] = %hd", k, temp[k]);
            j--;
            k--;
        }

        i--;
        printf("\nIndices after loop: i = %hd, j = %hd, k = %hd\n\n", i, j, k);
    }


    // Rewrite values from a temporary array to a digits array
    for (i = (us)(k + 1); i <= first_non_zero_idx; i++) digits[i] = temp[i];

//    i = first_non_zero_idx;
//    remainder = 0;
//    while (i >= prev_begin_idx || remainder) {
//        temp[i] = (us)((temp[i] + remainder) % 10);
//        remainder = (us)(remainder + temp[i] / 10);
//        i--;
//    }

    // Return an index of the first result's digit
    return (us)(k + 1);
}

void print_array(us* digits, us begin_idx, us end_idx) {
    for (us i = begin_idx; i < end_idx; i++) printf("%hu", digits[i]);
}
