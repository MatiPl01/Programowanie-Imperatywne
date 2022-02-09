#include <stdio.h>
#include <stdlib.h>


#define MATRIX_SIZE 6

typedef unsigned short us;

void read_coords(short *arr, unsigned count);
void apply_negations(us *matrix, const short *coords, unsigned count);
void negate_rectangle(us *matrix, short x1, short y1, short x2, short y2);
us count_black_squares(const us *matrix);
inline us map_coord_to_matrix(short c);
void swap(short *a, short *b);

//REMOVE ME
void print_coords(const short *arr, unsigned count);
void print_matrix(const us *matrix);


int main() {
    // Get input data and prepare variables
    unsigned N;
    scanf("%u", &N);
    short *coords = (short*)malloc(N * (sizeof(short) * 4));
    us *matrix = (us*)calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(us));
    read_coords((short*)coords, N);

    // REMOVE ME
    print_coords((short*)coords, N);

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
        for (short j = 0; j < 4; j++) {
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
        printf("\nBefore swap: (%hd, %hd), (%hd, %hd)\n", x1, y1, x2, y2);
        if (x1 > x2) swap(&x1, &x2);
        if (y1 > y2) swap(&y1, &y2);
        printf("\nAfter swap: (%hd, %hd), (%hd, %hd)\n", x1, y1, x2, y2);
        negate_rectangle(matrix, x1, y1, x2, y2);
        printf("\nAfter negating: (%hd, %hd), (%hd, %hd)\n", x1, y1, x2, y2);
        print_matrix(matrix);
    }
}

void swap(short *a, short *b) {
    short temp = *a;
    *a = *b;
    *b = temp;
}

inline us map_coord_to_matrix(short c) {
    return c < 0 ? (us)(MATRIX_SIZE / 2 + c) : (us)(MATRIX_SIZE / 2 + c - 1);
}

void negate_rectangle(us *matrix, short x1, short y1, short x2, short y2) {
    // As there is no 0 coordinate, we - TODO - comment
    us begin_y = map_coord_to_matrix(y1);
    us begin_x = map_coord_to_matrix(x1);
    us end_y = map_coord_to_matrix(y2);
    us end_x = map_coord_to_matrix(x2);

    printf("\nMapped coords:\n");
    printf("x1 = %hd -> begin_x = %hd\n", x1, begin_x);
    printf("y1 = %hd -> begin_y = %hd\n", y1, begin_y);
    printf("x2 = %hd -> end_x = %hd\n", x2, end_x);
    printf("y2 = %hd -> end_y = %hd\n", y2, end_y);

    for (us y = begin_y; y <= end_y; y = (us)(y + 1)) {
        for (us x = begin_x; x <= end_x; x = (us)(x + 1)) {
            printf("Loop x = %hd, y = %hd\n", x, y);
            *((matrix + y * MATRIX_SIZE) + x) = (us)!*((matrix + y * MATRIX_SIZE) + x);
        }
    }
}

us count_black_squares(const us *matrix) {
    us count = 0;

    for (us y = 0; y < MATRIX_SIZE; y++) {
        for (us x = 0; x < MATRIX_SIZE; x++) {
            count = (us)(count + *((matrix + y * MATRIX_SIZE) + x));
        }
    }

    return count;
}

// REMOVE ME
void print_coords(const short *arr, unsigned count) {
    for (unsigned i = 0; i < count; i++) {
        for (short j = 0; j < 4; j++) {
            printf("%hd ", *((arr + 4 * i) + j));
        }
        printf("\n");
    }
}

void print_matrix(const us *matrix) {
    for (us y = 0; y < MATRIX_SIZE; y = (us)(y + 1)) {
        for (us x = 0; x < MATRIX_SIZE; x = (us)(x + 1)) {
            printf("%hd ", *((matrix + y * MATRIX_SIZE) + x));
        }
        printf("\n");
    }
}
