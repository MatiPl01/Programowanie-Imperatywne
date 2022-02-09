#include <stdio.h>


int main() {
    // Get input
    unsigned long input;
    scanf("%lu", &input);

    // Loop in order to assess if an input value is a multiplication of
    // two following values of a Fibonacci sequence
    unsigned long a = 0, b = 1;
    while (a * b < input) {
        unsigned long temp = a;
        a = b;
        b = temp + a;
    }

    // Show a proper message showing whether an input is a multiplication
    // of two following Fibonacci sequence values or not
    if (a * b == input) printf("YES");
    else printf("NO");

    return 0;
}
