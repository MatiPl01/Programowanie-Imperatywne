#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef unsigned short us;

void add_romans(char rom_1[], char rom_2[], char rom_res[]);
us roman_to_decimal(const char rom[], us length);
void decimal_to_roman(us dec, char rom[]);

const char rom_num[][2] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
const us rom_len[] = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1};
const us dec_num[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};


int main() {
    // Create initial variables
    // There will be at most 12 characters in a roman number which is not greater than 1000
    char *rom_1 = (char*)calloc((char)(12), sizeof(char));
    char *rom_2 = (char*)calloc((char)(12), sizeof(char));
    // A result wil; have at most 13 characters as it's not greater than 2000
    char *rom_res = (char*)calloc((char)(13), sizeof(char));

    // Read input strings
    scanf("%s %s", rom_1, rom_2);

    // Calculate the resulting Roman number and print it out
    add_romans(rom_1, rom_2, rom_res);
    printf("%s", rom_res);

    // Release memory
    free(rom_1);
    free(rom_2);
    free(rom_res);

    return 0;
}


void add_romans(char rom_1[], char rom_2[], char rom_res[]) {
    // Convert both input roman numbers to decimal equivalents
    us dec_1 = roman_to_decimal(rom_1, (us)strlen(rom_1));
    us dec_2 = roman_to_decimal(rom_2, (us)strlen(rom_2));

    // Add both numbers and convert a result back to the roman number
    decimal_to_roman((us)(dec_1 + dec_2), rom_res);
}

us roman_to_decimal(const char rom[], us length) {
    us k, i, dec;
    dec = 0;
    k = i = 0;

    while (i < length) {
        // If roman digit which is currently checked has only one letter and the current
        // roman number's digit is the same as checked one, subtract a proper values
        // corresponding to the currently checked roman digit from a decimal number and
        // store this digit in an output 'rom' array
        if (rom_len[k] == 1 && rom[i] == rom_num[k][0]) {
            dec = (us)(dec + dec_num[k]);
            i = (us)(i + 1);
        // If roman digit which is currently checked has two letters, compare two subsequent
        // letters of a 'rom' input string
        } else if (rom_len[k] == 2 && rom[i] == rom_num[k][0] && rom[i + 1] == rom_num[k][1]) {
            dec = (us)(dec + dec_num[k]);
            i = (us)(i + 2);
        // Else advance a 'k' pointer which represents a roman digit that is checked
        } else {
            k = (us)(k + 1);
        }
    }

    return dec;
}

void decimal_to_roman(us dec, char rom[]) {
    us i = 0;
    us k = 0;

    while (dec > 0) {
        // Go to the next value if cannot subtract the current one as it is too large
        if (dec_num[k] > dec) {
            k = (us)(k + 1);
        } else {
            // Subtract a current roman digit value from a decimal number and rewrite
            // appropriate digits to the 'rom' array representing a roman number
            dec = (us)(dec - dec_num[k]);

            for (us j = 0; j < rom_len[k]; i = (us)(i + 1), j = (us)(j + 1)) {
                rom[i] = rom_num[k][j];
            }
        }
    }
}
