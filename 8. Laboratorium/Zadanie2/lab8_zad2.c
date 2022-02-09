#include <stdio.h>


typedef unsigned long long ul;

ul calc_sum(ul x);


int main() {
    // Read input data
    ul n;
    scanf("%llu", &n);

    // Calculate and print the result
    printf("%llu", calc_sum(n));

    return 0;
}


ul calc_sum(ul x) {
    ul total = ((x % 2) ? (x + 1) * (x + 1): x * x) / 4;
    ul idx = 1, step = 4, y;
    for (; idx < x; idx += step / 2, step *= 2) {
        y = (x - idx) / step + ((x - idx) % step ? 1 : 0);
        total += y * y;
    }
    return total;
}
