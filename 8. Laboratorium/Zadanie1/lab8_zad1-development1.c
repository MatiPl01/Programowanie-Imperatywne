#include <stdio.h>
#include <unistd.h>


typedef unsigned long long llu;

llu calc_X(llu S);


int main() {
    llu n, m;
    n = 0;
    m = 1000000;

    FILE *fp;

    chdir("F:\\Edukacja\\Studia\\2. semestr\\Programowanie Imperatywne\\Laboratoria\\8. Laboratorium\\Zadanie1");

    fp = fopen("results.txt", "w");

    if (fp == NULL){
        printf("Error Opening File\n");
        exit (0);
    }

    llu X;
    for (llu S = n; S <= m; S++) {
        X = calc_X(S);
        if (X) fprintf(fp, "%I64d %I64d\n", S, X);
        else fprintf(fp, "%I64d -1\n", S);
    }

    fclose(fp);

    return 0;
}


llu calc_X(llu S) {
    // Find the largest divisor which will be 111...1
    llu div = 0, mul = 1;
    while (div + mul <= S) {
        div += mul;
        mul *= 10;
    }

    llu res = 0, dgt;
    // In a loop find subsequent characters of the resulting number X
    while (div > 1) {
        dgt = S / div;
        if (dgt > 9) return 0;
        S %= div;
        div /= 10;
        res = 10 * res + dgt;
    }
    // Add the last one character
    if (S > 9) return 0;
    res = 10 * res + S;

    return res;
}
