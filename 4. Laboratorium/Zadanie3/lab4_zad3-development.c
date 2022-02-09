#include <stdio.h>


typedef unsigned short us;

void read_sq_matrix(us *matrix, us size);
void print_sq_matrix(us *matrix, us size);
void calc_matrix(us *source, us *output, us size, us radius);
us sum_in_area(us *matrix, us size, us radius, us row_idx, us col_idx);
short max(short a, short b);
short min(short a, short b);


int main() {
    // Prepare data
    us n, r;
    scanf("%hd", &n);
    scanf("%hd", &r);
    us F[n][n];
    us W[n][n];
    read_sq_matrix((us*)F, n);

    // REMOVE ME
    print_sq_matrix((us*)F, n);
    printf("\n%hd\n", sum_in_area((us*)F, n, r, 1, 1));

    // Calculate the W matrix
    calc_matrix((us*)F, (us*)W, n, r);

    // Print the result matrix
    print_sq_matrix((us*)W, n);

    return 0;
}


void read_sq_matrix(us *matrix, us size) {
    for (us i = 0; i < size; i++) {
        for (us j = 0; j < size; j++) {
            scanf("%hd", ((matrix + i * size) + j));
        }
    }
}

void print_sq_matrix(us *matrix, us size) {
    for (us i = 0; i < size; i++) {
        for (us j = 0; j < size; j++) {
            printf("%hd ", *((matrix + i * size) + j));
        }
        printf("\n");
    }
}

us sum_in_area(us *matrix, us size, us radius, us row_idx, us col_idx) {
    us sum = 0;
    for (short i = max((short)(row_idx - radius), 0); i <= min((short)(row_idx + radius), (short)(size - 1)); i++) {
        for (short j = max((short)(col_idx - radius), 0); j <= min((short)(col_idx + radius), (short)(size - 1)); j++) {
            sum = (us)(sum + *((matrix + i * size) + j));
        }
    }
    return sum;
}

short max(short a, short b) {
    return a > b ? a : b;
}

short min(short a, short b) {
    return a < b ? a : b;
}

void calc_matrix(us *source, us *output, us size, us radius) {
    us curr_sum;
    for (us i = 0; i < size; i++) {
        for (us j = 0; j < size; j++) {
            curr_sum = sum_in_area(source, size, radius, i, j);
            *((output + i * size) + j) = curr_sum;
        }
    }
}
