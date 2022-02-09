#include <stdio.h>
#include <stdlib.h>

#define INF 1 << 30
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ERROR_HANDLE_PLACEHOLDER (exit(1))

void get_input(unsigned* arr, unsigned n) {
    for (unsigned i = 0; i < n; ++i) {
        if (scanf("%u", &arr[i]) != 1) ERROR_HANDLE_PLACEHOLDER;
    }
}

int main(void) {
    unsigned r[50], g[50], b[50];
    unsigned n;

    if (scanf("%u", &n) != 1) return 1;

    get_input(r, n);
    get_input(g, n);
    get_input(b, n);

    unsigned arr[110][8];
    for (unsigned i = 0; i <= n; i++) {
        for (unsigned j = 0; j <= 7; j++) {
            arr[i][j] = INF;
        }
    }
    arr[0][0] = 0;

    for (unsigned i = 0; i < n; i++) {
        for (unsigned j = 0; j <= 7; j++) {
            arr[i + 1][j | 1] = MIN(arr[i + 1][j | 1], arr[i][j] + g[i] + b[i]);
            arr[i + 1][j | 2] = MIN(arr[i + 1][j | 2], arr[i][j] + r[i] + b[i]);
            arr[i + 1][j | 4] = MIN(arr[i + 1][j | 4], arr[i][j] + r[i] + g[i]);
        }
    }
    unsigned j = 0;
    for (unsigned i = 0; i < n; i++) {
        if (r[i]) j |= 1;
        if (g[i]) j |= 2;
        if (b[i]) j |= 4;
    }

    if (arr[n][j] >= (1 << 30))
        printf("-1 \n");
    else
        printf("%d \n", arr[n][j]);

    return 0;
}
