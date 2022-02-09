#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef unsigned short us;

void read_matrix(int *matrix, us size);

long max_neighbours_sum(const int *matrix, us size, us k);
long max_neighbours_sum_td(const int *matrix, us size, us k);
long max_neighbours_sum_lr(const int *matrix, us size, us k);
long max_neighbours_sum_tl_br(const int *matrix, us size, us k);
long max_neighbours_sum_tr_bl(const int *matrix, us size, us k);

long max(const long arr[], us size);


int main() {
    // Prepare variables and read data
    us n, k;
    scanf("%hd %hd", &n, &k);
    int *T = (int*)malloc(sizeof(int) * n * n);
    read_matrix((int*)T, n);

    // Calculate the maximum sum of k neighbours in an array of ints
    printf("%li", max_neighbours_sum((int*)T, n, k));

    // Release memory
    free(T);

    return 0;
}


void read_matrix(int *matrix, us size) {
    for (us i = 0; i < size * size; i = (us)(i + 1)) {
        scanf("%d", (matrix + i));
    }
}

long max_neighbours_sum(const int *matrix, us size, us k) {
    long results[] = {
            // Max sum of neighbours placed vertically
            max_neighbours_sum_td(matrix, size, k),
            // Max sum of neighbours placed horizontally
            max_neighbours_sum_lr(matrix, size, k),
            // Max sum of neighbours placed on a top-left bottom-right diagonal
            max_neighbours_sum_tl_br(matrix, size, k),
            // Max sum of neighbours placed on a top-right bottom-left diagonal
            max_neighbours_sum_tr_bl(matrix, size, k),
    };
    return max(results, 4);
}

long max_neighbours_sum_td(const int *matrix, us size, us k) {
    long curr_sum, max_sum;
    max_sum = LONG_MIN;

    for (us j = 0; j < size; j = (us)(j + 1)) {
        // Calculate the first sum
        curr_sum = *(matrix + j);
        for (us i = 1; i < k; i = (us)(i + 1)) {
            curr_sum += *((matrix + i * size) + j);
        }
        if (curr_sum > max_sum) max_sum = curr_sum;

        // Slide to bottom the rectangle and drop the first (uppermost value)
        // and add a new value which was included in the rectangle (bottommost one)
        for (us i = 1; i < size; i = (us)(i + 1)) {
            curr_sum -= *((matrix + (i - 1) * size) + j);
            curr_sum += *((matrix + ((i + k - 1) % size) * size) + j);
            if (curr_sum > max_sum) max_sum = curr_sum;
        }
    }

    return max_sum;
}

long max_neighbours_sum_lr(const int *matrix, us size, us k) {
    long curr_sum, max_sum;
    max_sum = LONG_MIN;

    for (us i = 0; i < size; i = (us)(i + 1)) {
        // Calculate the first sum
        curr_sum = *(matrix + i * size);
        for (us j = 1; j < k; j = (us)(j + 1)) {
            curr_sum += *((matrix + i * size) + j);
        }
        if (curr_sum > max_sum) max_sum = curr_sum;

        // Slide to right the rectangle and drop the first (leftmost value)
        // and add a new value which was included in the rectangle (rightmost one)
        for (us j = 1; j < size; j = (us)(j + 1)) {
            curr_sum -= *((matrix + i * size) + (j - 1));
            curr_sum += *((matrix + i * size) + (j + k - 1) % size);
            if (curr_sum > max_sum) max_sum = curr_sum;
        }
    }

    return max_sum;
}

long max_neighbours_sum_tl_br(const int *matrix, us size, us k) {
    long curr_sum, max_sum;
    max_sum = LONG_MIN;

    for (us j = 0; j < size; j = (us)(j + 1)) {
        // Calculate the first sum
        curr_sum = *(matrix + j);
        for (us i = 1; i < k; i = (us)(i + 1)) {
            curr_sum += *((matrix + i * size) + ((j + i) % size));
        }
        if (curr_sum > max_sum) max_sum = curr_sum;

        // Slide to bottom-right the rectangle and drop the first (top-left value)
        // and add a new value which was included in the rectangle (bottom-right one)
        for (us i = 1; i < size; i = (us)(i + 1)) {
            curr_sum -= *((matrix + (i - 1) * size) + ((j + i - 1) % size));
            curr_sum += *((matrix + ((i + k - 1) % size) * size) + ((j + i + k - 1) % size));
            if (curr_sum > max_sum) max_sum = curr_sum;
        }
    }

    return max_sum;
}

long max_neighbours_sum_tr_bl(const int *matrix, us size, us k) {
    long curr_sum, max_sum;
    max_sum = LONG_MIN;

    for (us j = 0; j < size; j = (us)(j + 1)) {
        // Calculate the first sum
        curr_sum = *(matrix + j);
        for (us i = 1; i < k; i = (us)(i + 1)) {
            curr_sum += *((matrix + i * size) + (size + j - i) % size);
        }
        if (curr_sum > max_sum) max_sum = curr_sum;

        // Slide to bottom-right the rectangle and drop the first (top-left value)
        // and add a new value which was included in the rectangle (bottom-right one)
        for (us i = 1; i < size; i = (us)(i + 1)) {
            curr_sum -= *((matrix + (i - 1) * size) + ((size + j - i + 1) % size));
            curr_sum += *((matrix + ((i + k - 1) % size) * size) + ((size + j - i - k + 1) % size));
            if (curr_sum > max_sum) max_sum = curr_sum;
        }
    }

    return max_sum;
}

long max(const long arr[], us size) {
    long result = LONG_MIN;

    for (us i = 0; i < size; i = (us)(i + 1)) {
        if (arr[i] > result) result = arr[i];
    }

    return result;
}
