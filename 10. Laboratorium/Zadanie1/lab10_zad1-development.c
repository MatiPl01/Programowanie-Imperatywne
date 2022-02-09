#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAXDECDIGITS 30
#define MAXBINDIGITS 100

typedef unsigned short us;

void store_digits(const char num[], us *digits, us n);
us dec_to_bin(us *digits, us *binary, us n);
us to_negabinary(us *binary, us msbi);

unsigned divmod(us *digits, us n, us divisor);


void print_arr(us *digits, us n);
void print_arr_bin(us *binary, us msbi);


int main() {
    // Get input data
    us n;
    char num[MAXDECDIGITS];
    scanf("%s", num);
    n = (us)strlen(num);

    // Create an array of digits
    us *digits = (us*)malloc(n * sizeof(us));
    store_digits(num, digits, n);

//    print_arr(digits, n);
//    printf("Reminder: %u\n", divmod(digits, n, 2));
//    print_arr(digits, n);

    // Convert a decimal number to the binary number
    us *binary = (us*)calloc(MAXBINDIGITS, sizeof(us));
    us msbi = dec_to_bin(digits, binary, n); // msbi - most significant bit index

    if (msbi < MAXBINDIGITS) {
        printf("Most significant bit index: %hd\n", msbi);
        printf("Array of binary digits:\n");
        print_arr_bin(binary, msbi);

        // Convert a binary number to the negabinary
        msbi = to_negabinary(binary, msbi);

        if (msbi < MAXBINDIGITS) {
            printf("Array of negabinary number digits:\n");
            print_arr_bin(binary, msbi);

            for (us i = 0; i <= msbi; i = (us) (i + 1)) {
                if (binary[i]) printf("%hd ", i);
            }
            printf("\n");
        } else printf("NO");
    } else printf("NO");

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

        printf("\nDivided by 2\n");
        printf("Digits array now: \n");
        print_arr(digits, n);
        printf("Binary array now: \n");
        print_arr_bin(binary, (us)(idx - 1));

        while (i < n && !digits[i]) i++;
    }
    // Return an index of the most significant bit
    return (us)(idx - 1);
}


us to_negabinary(us binary[], us msbi) {
    // Add a 10101010...1010 mask to the binary number
    short i = 0;
    us carry = 0, sum;
    while (carry || i <= msbi) {
        if (i == MAXBINDIGITS) return MAXBINDIGITS;
        sum = (us)(binary[i] + i % 2 + carry);
        binary[i] = sum % 2;
        carry = sum / 2;
        i++;
    }
    msbi = (us)(i - 1);

    printf("\nNew most significant bit index: %hd\n", msbi);
    printf("Binary array after adding a mask: \n");
    print_arr_bin(binary, msbi);
    printf("\n");

    // Perform xor operation between the 10101010...1010 and
    // a binary number in a binary[] array
    for (us j = 0; j <= msbi; j = (us)(j + 1)) {
        binary[j] = binary[j] != j % 2;
    }

    return msbi;
}


void print_arr(us *digits, us n) {
    for (us i = 0; i < n; i = (us)(i + 1)) {
        printf("%hd ", digits[i]);
    }
    printf("\n");
}


void print_arr_bin(us binary[], us msbi) {
    for (us i = 0; i <= msbi; i = (us)(i + 1)) {
        printf("%hd ", binary[i]);
    }
    printf("\n");
}