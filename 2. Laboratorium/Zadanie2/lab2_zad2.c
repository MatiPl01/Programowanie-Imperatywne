#include <stdio.h>
#include <stdbool.h>


const char* base_digits = "0123456789ABCDEF";


bool is_narcissistic(char* n_base, unsigned long n_dec, unsigned short num_digits);
unsigned long power(unsigned short d, unsigned short exp);
unsigned short hex_digit_to_dec(unsigned char hex_digit);
void dec_to_base(unsigned long n, unsigned short base, unsigned short num_digits, char* result_chars);


int main() {
    unsigned short m, b;
    scanf("%hd %hd", &m, &b);

    // Store a decimal value of the same value as the lowest number in the specified
    // base which has the specified number of digits
    unsigned long min_dec = power(b, m - 1); // e.g. b = 5, m = 4  ->  1000(5) = 1 * 5^3 = b^(m - 1)
    // Do the same for the maximum number in the specified base which has the number
    // of digits equal to m
    unsigned long max_dec = 0;  // e.g. b = 5, m = 4  ->  4444(5) = 4 * 5^0 + 4 * 5^1 + 4 * 5^2 + 4 * 5^3
    for (unsigned short i = 0; i < m; ++i) max_dec += power(b, i);
    max_dec *= (b - 1);

    // Prepare variables
    bool found_flag = false;
    char n_base[m];

    // Loop over all the numbers in a specified range (from the min_dec to the max_dec)
    for (unsigned long num = min_dec; num <= max_dec; ++num) {
        dec_to_base(num, b, m, n_base);

        if (is_narcissistic(n_base, num, m)) {
            found_flag = true;
            // Print an array of number's digits
            // Don't know why 'printf("%s", n_base);' prints some gibberish at the end of
            // the array printed. I allocate 'm' indices and always fill all the allocated space.
            // so I feel this should work but, unfortunately, doesn't.
            for (unsigned short i = 0; i < m; i++) printf("%c", n_base[i]);
            printf(" ");
        }
    }

    if (!found_flag) printf("NO");

    return 0;
}


bool is_narcissistic(char* n_base, unsigned long n_dec, unsigned short num_digits) {
    // Check if a number is narcissistic (whether a sum of digits to the power
    // of count is equal to the n number
    unsigned long powers_sum = 0;
    for (unsigned short i = 0; i < num_digits; ++i) {
        powers_sum += power(hex_digit_to_dec(n_base[i]), num_digits);
        // Return false if we already know that a sum of digits' powers exceeds
        // a decimal representation of the number passed
        if (powers_sum > n_dec) return false;
    }
    if (powers_sum == n_dec) return true;
    return false;
}


// This only works for our cases when we pass small number as arguments
unsigned long power(unsigned short d, unsigned short exp) {
    unsigned long result = 1;
    // Calculate d ^ exp
    for (unsigned short i = 0; i < exp; ++i) result *= d;
    return result;
}


void dec_to_base(unsigned long n, unsigned short base, unsigned short num_digits, char* result_chars) {
    for (unsigned short i = num_digits; i > 0; i--) {
        result_chars[i - 1] = base_digits[n % base];
        n /= base;
    }
}


unsigned short hex_digit_to_dec(unsigned char hex_digit) {
    if (hex_digit >= '0' && hex_digit <= '9') return hex_digit - '0';
    if (hex_digit >= 'A' && hex_digit <= 'F') return hex_digit - 'A' + 10;
    if (hex_digit >= 'a' && hex_digit <= 'f') return hex_digit - 'a' + 10;
    return -1;
}
