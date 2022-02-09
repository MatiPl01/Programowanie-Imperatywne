#include <stdio.h>


typedef unsigned short us;

//void swap(int *ptr1, int *ptr2);
void print_array(int arr[], us size);
void read_matrix(int *matrix, us size);
void print_matrix(int *matrix, us size);
us merge_sorted_arrays(int *matrix, int target[], int temp[], us size);
us linearize(int *matrix, us size, int target[]);
//us merge(int source[], us source_end_idx, int target[], us step);


int main() {
    // Prepare data
    us N;
    scanf("%hd", &N);
    int t1[N][N];
    int t2[N * N];
    int temp[N * N];

    // Read matrix values
    read_matrix((int*)t1, N);
    print_matrix((int*)t1, N);

    // Rewrite values of a matrix at first to the linear array
    // and perform merging on adjacent rows
    us last_idx = merge_sorted_arrays((int*)t1, t2, temp, N);

    // Print a result
    print_array(t2, (us)(last_idx + 1));

    return 0;
}


void read_matrix(int *matrix, us size) {
    for (us i = 0; i < size; i++) {
        for (us j = 0; j < size; j++) {
            scanf("%d", ((matrix + i * size) + j));
        }
    }
}

void print_matrix(int *matrix, us size) {
    for (us i = 0; i < size; i++) {
        printf("\n[ ");
        for (us j = 0; j < size; j++) {
            printf("%d ", *((matrix + i * size) + j));
        }
        printf("]");
    }
    printf("\n");
}

void print_array(int arr[], us size) {
    for (us i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

us linearize(int *matrix, us size, int target[]) {
    us i = 1, j = 0, k = 0;
    for (; i < size; i = (us)(i + 2)) {
        us first_idx = 0;
        us second_idx = 0;
        int first_val, second_val;
        printf("\nMain linearize loop: i %hd, k %hd, first_idx %hd, second_idx %hd\n", i, k, first_idx, second_idx);

        // Merge sorted rows
        while (first_idx < size && second_idx < size) {
            first_val = *((matrix + (i - 1) * size) + first_idx);
            second_val = *((matrix + i * size) + second_idx);
            // If the first values is lower than the second one
            if (first_val < second_val) {
                target[k] = first_val;
                first_idx++;
                // Check if the next value is the same as the previous one. If it is the same,
                // skip this value.
//                while (first_idx < size && *((matrix + (i - 1) * size) + first_idx) == first_val) first_idx++;
            // If the second values is lower than the first one
            } else {
//            else if (first_val > second_val) {
                target[k] = second_val;
                second_idx++;
                // Skip duplicates (the same as above)
//                while (second_idx < size && *((matrix + i * size) + second_idx) == second_val) second_idx++;
                // If values are equal, store only the first occurrence
                // and skip all the repeated values
//            } else {
//                target[k] = first_val;
//                while (first_idx < size && *((matrix + (i - 1) * size) + first_idx) == first_val) first_idx++;
//                while (second_idx < size && *((matrix + i * size) + second_idx) == second_val) second_idx++;
//            }
            }
            k++;
        }

        printf("\nAfter linearize merging: i %hd, k %hd, first_idx %hd, second_idx %hd\n", i, k, first_idx, second_idx);

        // Rewrite remaining values of one of the rows
        for (; first_idx < size;  first_idx++)  target[k++] = *((matrix + (i - 1) * size) + first_idx);
        for (; second_idx < size; second_idx++) target[k++] = *((matrix + i * size) + second_idx);
    }

    // If there was odd number of rows, rewrite the last one separately
    if (--i < size) {
        printf("\nLast linearize loop i: %hd\n", i);
        for (j = 0; j < size; j++) {
            target[k++] = *((matrix + i * size) + j);
        }
    }

    // Return the last index of a linearized array
    return (us)(k - 1);
}

//us merge(int source[], us source_end_idx, int target[], us step) {
//    us i, j, k;
//    i = k = 0;
//    j = step;
//
//    while (j < source_end_idx) {
//
//        while (i < step && j < source_end_idx && j < step * 2) {
//            if (source[i] < source[j]) {
//                target[k] = source[i++];
//            } else {
//                target[k] = source[j++];
//            }
//            k++;
//        }
//    }
//}

us merge_sorted_arrays(int *matrix, int target[], int temp[], us size) {
    us end_idx = linearize(matrix, size, target);
    // To remove
    print_array(target, (us)(end_idx + 1));

//    us step = size;
//    do {
//        step = (us)(2 * step);
//        end_idx = merge(target, end_idx, temp, step);
//        // Swap pointers in order to make a temporary array a target
//        swap(target, temp);
//    } while (step < size);
    return end_idx;
}

void swap(int *ptr1, int *ptr2) {
    int temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}
