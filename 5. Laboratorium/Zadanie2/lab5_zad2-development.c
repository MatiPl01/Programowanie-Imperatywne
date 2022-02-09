#include <stdio.h>


#define ARR_SIZE 100

void fill_matrix_spiral(int *matrix, int size);
void print_matrix(const int *matrix, int size);


int main() {
    // Get input data
    int n;
    int T[ARR_SIZE][ARR_SIZE];
    scanf("%d", &n);

    // Fill an array with values on spiral
    fill_matrix_spiral((int*)T, n);

    // Print an array which was filled
    print_matrix((int*)T, n);

    return 0;
}


void fill_matrix_spiral(int *matrix, int size) {
    int n = 1;

    for (int begin_idx = 0; begin_idx < (size + 1) / 2; begin_idx++) {
        printf("\nBegin idx: %d\n", begin_idx);
        for (int x = begin_idx; x < size - begin_idx; x++)
            *((matrix + size * begin_idx) + x) = n++;
        for (int y = begin_idx + 1; y < size - begin_idx; y++)
            *((matrix + size * y) + (size - begin_idx - 1)) = n++;
        for (int x = size - begin_idx - 2; x >= begin_idx; x--)
            *((matrix + size * (size - begin_idx - 1)) + x) = n++;
        for (int y = size - begin_idx - 2; y > begin_idx; y--)
            *((matrix + size * y) + begin_idx) = n++;
        print_matrix(matrix, size);
        printf("\n\n");
    }
}

void print_matrix(const int *matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", *((matrix + i * size) + j));
        }
        printf("\n");
    }
}