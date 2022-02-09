#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAXDECDIGITS 30
#define MAXBINDIGITS 100

typedef unsigned short us;

void store_digits(const char num[], us *digits, us n);
us dec_to_bin(us *digits, us *binary, us n);
void copy_array(const us *arr, us *result, us n);

us negabinary(us *binary, us msbi, bool start_with_one);

void print_ATMs(const us *binary, us msbi);
void borrow(us *binary, us msbi);
void repay(us *binary, us msbi);


int main() {
    // Get input data
    us n;
    char num[MAXDECDIGITS];
    scanf("%s", num);
    n = (us)strlen(num);

    // Create an array of digits
    us *digits = (us*)malloc(n * sizeof(us));
    store_digits(num, digits, n);

    // Convert a decimal number to the binary number
    us *binary = (us*)calloc(MAXBINDIGITS, sizeof(us));
    us msbi = dec_to_bin(digits, binary, n); // msbi - most significant bit index

    // Check if we can borrow the specified amount of money
    // If yes, print the numbers of subsequent cash machines
    if (msbi < MAXBINDIGITS) {
        borrow(binary, msbi);
        repay(binary, msbi);
    } else {
        printf("NO");
    }

    // Release memory
    free(digits);
    free(binary);

    return 0;
}


void store_digits(const char num[], us *digits, us n) {
    for (us i = 0; i < n; i = (us)(i + 1)) {
        digits[i] = (us)(num[i] - '0');
    }
}


unsigned divmod(us *digits, us n, us divisor) {
    /* This function divides a number represented by an array of decimal digits
     * and stores a result in the same array. After such a division, the remainder
     * is returned.
     * This function works only for positive divisors and positive numbers divided.
     */
    unsigned curr = 0, temp = 0;
    // Divide a number stored in a digits array
    for (us i = 0; i < n; i = (us)(i + 1)) {
        curr += digits[i];
        if (curr >= divisor) {
            temp = curr / divisor;
            curr -= divisor * temp;
        } else {
            temp = 0;
        }
        digits[i] = (us)temp;
        curr *= 10;
    }

    return curr / 10;
}


us dec_to_bin(us *digits, us binary[], us n) {
    short idx = 0, i = 0; // i is the index of the first non-zero
    // digit of a number stored in the digits array

    // Loop till there are some digits remaining
    while (i < n) {
        if (idx == MAXBINDIGITS) return MAXBINDIGITS;
        us dgt = (us)divmod(digits, n, 2);
        binary[idx++] = dgt;
        while (i < n && !digits[i]) i++;
    }
    // Return an index of the most significant bit
    return (us)(idx - 1);
}


us negabinary(us binary[], us msbi, bool start_with_one) {
    // Add a 10101010...1010 mask to the binary number
    short i = 0;
    us carry = 0, sum;
    while (carry || i <= msbi) {
        if (i == MAXBINDIGITS) return MAXBINDIGITS;
        sum = (us)(binary[i] + (i + start_with_one) % 2 + carry);
        binary[i] = sum % 2;
        carry = sum / 2;
        i++;
    }
    msbi = (us)(i - 1);

    // Perform xor operation between the 10101010...1010 and
    // a binary number in a binary[] array
    for (us j = 0; j <= msbi; j = (us)(j + 1)) {
        binary[j] = binary[j] != (j + start_with_one) % 2;
    }

    return msbi;
}


void copy_array(const us *arr, us *result, us n) {
    for (us i = 0; i < n; i = (us)(i + 1)) {
        result[i] = arr[i];
    }
}


void print_ATMs(const us *binary, us msbi) {
    for (us i = 0; i <= msbi; i = (us) (i + 1)) {
        if (binary[i]) printf("%hd ", i);
    }
    printf("\n");
}


void borrow(us *binary, us msbi) {
    // Create a copy od the binary number array
    us *binary_cp = (us*)malloc(MAXBINDIGITS * sizeof(us));
    copy_array(binary, binary_cp, MAXBINDIGITS);

    // Convert a binary number to the negabinary
    msbi = negabinary(binary_cp, msbi, false);

    // Print the result
    if (msbi < MAXBINDIGITS) print_ATMs(binary_cp, msbi);
    else printf("NO\n");

    // Release memory
    free(binary_cp);
}


void repay(us *binary, us msbi) {
    // Create a copy od the binary number array
    us *binary_cp = (us*)malloc(MAXBINDIGITS * sizeof(us));
    copy_array(binary, binary_cp, MAXBINDIGITS);

    // Convert a binary number to the negabinary
    msbi = negabinary(binary_cp, msbi, true);

    // Print the result
    if (msbi < MAXBINDIGITS) print_ATMs(binary_cp, msbi);
    else printf("NO\n");

    // Release memory
    free(binary_cp);
}
