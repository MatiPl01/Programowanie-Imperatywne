#include <stdio.h>
#include <stdbool.h>

#define SIZE 100


bool can_weigh(long mass, unsigned *weights, unsigned short idx);


int main() {
    // Get input data
    unsigned short n;
    long w;
    scanf("%hd %li", &n, &w);
    unsigned weights[SIZE];
    unsigned long sum_weights = 0;
    for (unsigned short i = 0; i < n; i++) {
        scanf("%u", &weights[i]);
        sum_weights += weights[i];
    }

    // Check if the mass passed is possible to be weighed using specified weights
    if (w > sum_weights) printf("NO");
    else {
        if (can_weigh(w, weights, n)) printf("YES");
        else printf("NO");
    }

    return 0;
}


bool can_weigh(long mass, unsigned *weights, unsigned short idx) {
    // Return true if the remaining mass is equal to 0 (that means the weight is in balance)
    if (mass == 0) return true;
    // Return false if all the weights were exhausted and we couldn't weigh a mass
    if (idx == 0) return false;
    idx -= 1;
           // We can put a weight on the second scale pan (this without the mass)
    return can_weigh(mass - (long)weights[idx], weights, idx) ||
           // Or put a weight on the same scale pan as our mass is placed on
           can_weigh(mass + (long)weights[idx], weights, idx) ||
           // Or we can simply omit a current weight and go to the next one
           can_weigh(mass, weights, idx);
}
