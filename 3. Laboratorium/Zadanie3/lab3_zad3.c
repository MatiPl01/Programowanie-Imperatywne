#include <stdio.h>
#include <stdbool.h>


unsigned long sum_digits_squares(unsigned long num);
bool is_one_quadrant(unsigned long num);
bool is_prime(unsigned long num);


int main() {
    unsigned long L, U, K;
    scanf("%lu %lu %lu", &L, &U, &K);

    // Look for the Kth one-quadrant prime number
    unsigned long count = 0;
    for (unsigned long n = L; n <= U; n++) {
        if (is_prime(n) && is_one_quadrant(n)) count++;
        if (count == K) {
            printf("%lu", n);
            return 0;
        }
    }
    // Print -1 if not found
    printf("%d", -1);

    return 0;
}


unsigned long sum_digits_squares(unsigned long num) {
    unsigned long sum = 0;

    while (num) {
        unsigned digit = num % 10;
        num = num / 10;
        sum += digit * digit;
    }

    return sum;
}

bool is_one_quadrant(unsigned long num) {
    while (num != 4) {
        num = sum_digits_squares(num);
        if (num == 1) return true;
    }
    return false;
}

bool is_prime(unsigned long num) {
    if (num == 2 || num == 3) return true;
    if (!(num % 2) || !(num % 3) || num < 2) return false;
    for (unsigned long i = 5; i * i <= num; i += 6) {
        if (!(num % i) || !(num % (i + 2))) return false;
    }
    return true;
}
