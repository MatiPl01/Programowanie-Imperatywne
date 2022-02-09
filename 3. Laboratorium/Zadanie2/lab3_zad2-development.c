#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


typedef unsigned long values_type;

int compare_fn(const void *a, const void *b);
unsigned long count_pairs(values_type arr[], unsigned long length, unsigned long delta);
bool has_pair(const values_type arr[], unsigned long length, unsigned long idx, unsigned long delta);


int main() {
    // Read and store input data
    unsigned long N, K;
//    scanf("%u %u", &N, &K);
//    values_type A[N];
//    for (unsigned i = 0; i < N; ++i) scanf("%lu", &A[i]);


    FILE *fp;
//    char buff[255];

    chdir("F:\\Edukacja\\Studia\\2. semestr\\Programowanie Imperatywne\\Laboratoria\\3. Laboratorium\\Zadanie2");

    fp = fopen("test-case.txt", "r");

    if (fp == NULL){
        printf("Error Reading File\n");
        exit (0);
    }

    fscanf(fp, "%lu %lu", &N, &K);
    printf("N: %lu, K: %lu\n\n", N, K);
    values_type A[N];
    for (unsigned long i = 0; i < N; ++i) fscanf(fp, "%lu", &A[i]);

    fclose(fp);


    // Print a result (a number of numbers which have a pair)
    printf("\n\nResult: %lu", count_pairs(A, N, K));

    return 0;
}


int compare_fn(const void *a, const void *b) {
    if (*(values_type*)a < *(values_type*)b) return -1;
    return 1;
}

unsigned long count_pairs(values_type arr[], unsigned long length, unsigned long delta) {
    if (length < 2 || delta == 0) return 0;
    // Sort an array to ease further operations
    qsort(arr, length, sizeof(values_type), compare_fn);

//    unsigned first_repeated_idx = move_repeated_values(arr, length);


//    printf("Array:\n");
//    for (unsigned i=0; i<length; i++) {
//        printf("%lu ", arr[i]);
//    }
//    printf("\n");

//    values_type with_pairs[length];


    unsigned long pairs_count = 0;
    // Move forward and check subsequent elements if they have pairs
    for (unsigned long i = 0; i < length; ++i) {
        if (has_pair(arr, length, i, delta)) {

//            with_pairs[pairs_count] = arr[i];

            pairs_count++;
            // Do not check whether the same value has a pair in an array again
            // as we already checked it. To improve performance simply skip all
            // the repetitions of this value and increment pairs_count counter.
            while (i+1 < length && arr[i+1] == arr[i]) {
//                printf("\nSkipping: %lu", arr[i]);

//                with_pairs[pairs_count] = arr[i];

                pairs_count++;
                i++;
            }
        }
        else {
            // Skip these elements which have no pairs
            while (i+1 < length && arr[i+1] == arr[i]) i++;
        }
    }

//    printf("\n");
//    for (unsigned i=0; i<pairs_count; i++) printf("\n%lu", with_pairs[i]);

    return pairs_count;
}

bool has_pair(const values_type arr[], unsigned long length, unsigned long idx, unsigned long delta) {
    // Current value
    unsigned long curr_val = arr[idx];
    // Check if an element has a pair on the right side
    long i = (long)idx + 1;
    while (i < length && arr[i] - curr_val <= delta) {
        if (arr[i] != curr_val) return true;
        i++;
    }
    // Check if an element has a pair on the left side
    i = (long)idx - 1;
    while (i >= 0 && curr_val - arr[i] <= delta) {
        if (arr[i] != curr_val) return true;
        i--;
    }
    // If no pair was found in the desired range, return false
    return false;
}
