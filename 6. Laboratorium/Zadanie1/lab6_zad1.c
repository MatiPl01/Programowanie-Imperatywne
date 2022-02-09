#include <stdio.h>
#include <stdlib.h>


#define MATRIX_SIZE 200

typedef unsigned short us;

void read_coords(short *arr, unsigned count);
void apply_negations(us *matrix, const short *coords, unsigned count);
void negate_rectangle(us *matrix, short x1, short y1, short x2, short y2);
us count_black_squares(const us *matrix);


int main() {
    // Get input data and prepare variables
    unsigned N;
    scanf("%u", &N);
    short *coords = (short*)malloc(N * (sizeof(short) * 4));
    us *matrix = (us*)calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(us));
    read_coords((short*)coords, N);

    // Apply negations based on inputted coordinates of rectangles
    apply_negations((us*)matrix, (short*)coords, N);

    // Print the resulting number of black squares after applying negations
    printf("%hd", count_black_squares((us*)matrix));

    // Release memory
    free(coords);
    free(matrix);

    return 0;
}


void read_coords(short *arr, unsigned count) {
    for (unsigned i = 0; i < count; i++) {
        for (us j = 0; j < 4; j = (us)(j + 1)) {
            scanf("%hd", ((arr + 4 * i) + j));
        }
    }
}

void apply_negations(us *matrix, const short *coords, unsigned count) {
    short x1, y1, x2, y2;
    for (unsigned i = 0; i < count; i++) {
        x1 = *((coords + 4 * i) + 0);
        y1 = *((coords + 4 * i) + 1);
        x2 = *((coords + 4 * i) + 2);
        y2 = *((coords + 4 * i) + 3);
        negate_rectangle(matrix, x1, y1, x2, y2);
    }
}

void negate_rectangle(us *matrix, short x1, short y1, short x2, short y2) {
    // Negate all squares which are overlapped by the current rectangle
    for (us i = (us)(MATRIX_SIZE / 2 + y1); i < (us)(MATRIX_SIZE / 2 + y2); i = (us)(i + 1)) {
        for (us j = (us)(MATRIX_SIZE / 2 + x1); j < (us)(MATRIX_SIZE / 2 + x2); j = (us)(j + 1)) {
            *((matrix + i * MATRIX_SIZE) + j) = !*((matrix + i * MATRIX_SIZE) + j);
        }
    }
}


us count_black_squares(const us *matrix) {
    us count = 0;

    // Count all ones (black squares)
    for (us y = 0; y < MATRIX_SIZE; y++) {
        for (us x = 0; x < MATRIX_SIZE; x++) {
            count = (us)(count + *((matrix + y * MATRIX_SIZE) + x));
        }
    }
    return count;
}
