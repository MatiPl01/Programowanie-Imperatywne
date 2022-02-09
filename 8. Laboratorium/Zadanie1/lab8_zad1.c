#include <stdio.h>


typedef unsigned long long llu;

llu calc_X(llu S);


int main() {
    // Read input data
    llu S;
    scanf("%llu", &S);

    // Calculate the resulting number
    llu res = calc_X(S);
    if (res) printf("%llu\n", res);
    else printf("%d\n", -1);

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
