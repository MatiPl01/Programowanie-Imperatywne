#include <stdio.h>


void build_max_heap(unsigned long arr[], unsigned length);
unsigned long remove_max_from_heap(unsigned long arr[], unsigned *length);
void insert_to_max_heap(unsigned long arr[], unsigned *length, unsigned long value);
void heapify_down(unsigned long arr[], unsigned curr_idx, unsigned end_idx);
void heapify_up(unsigned long arr[], unsigned curr_idx, unsigned end_idx);
void swap(unsigned long *a, unsigned long *b);
inline unsigned left_child(unsigned curr_idx);
inline unsigned right_child(unsigned curr_idx);
inline unsigned parent(unsigned curr_idx);


int main() {
    // Read and store input data
    unsigned N, K;
    scanf("%u %u", &N, &K);
    unsigned long A[N];
    for (unsigned i = 0; i < N; ++i) scanf("%lu", &A[i]);

    // Build a max heap in order to have an easy access to the greatest value
    build_max_heap(A, N);
    // Loop till the heap exists (we won't insert 0 values when we get 0 after
    // dividing the max value by 2 or we haven't exhausted all the possible
    // operations (that means we can still do some divisions)
    // We will decrease a number of the remaining operations in each loop after
    // performing a comparison (see K-- below).
    while (K-- > 0 && N > 0) {
        unsigned long max_val = remove_max_from_heap(A, &N);
        unsigned long new_val = max_val / 2;

        printf("Removed value: %li\n", max_val);
        printf("New value: %li\n", new_val);
        printf("Array after removal:\n");
        for (unsigned i=0; i<N; ++i) printf("%li ", A[i]);
        printf("\nArray length after removal: %u\n", N);

        // Insert max_val/2 to a heap if is non zero (as it is not necessary to
        // store a zero value because it will not affect a final sum of values)
        if (new_val != 0) {
            insert_to_max_heap(A, &N, new_val);

            printf("\nArray after insertion:\n");
            for (unsigned i=0; i<N; ++i) printf("%li ", A[i]);
            printf("\nArray length after insertion: %u\n", N);
        }
    }

    // Get a sum of values remaining in a heap
    unsigned long sum = 0;
    for (unsigned i = 0; i < N; ++i) {
        sum += A[i];
    }

    // Print the result
    printf("\nSum: %lu", sum);

    return 0;
}


void build_max_heap(unsigned long arr[], unsigned length) {
    for (long int i = (long int)length / 2; i >= 0; i--) {
        heapify_down(arr, (unsigned)i, length);
    }
}

unsigned long remove_max_from_heap(unsigned long arr[], unsigned *length) {
    // If a heap array is not empty (we haven't removed the last
    // element from a heap), remove the first node and fix a heap
    if (length > 0) {
        // Store the max value which will be removed
        unsigned long removed = arr[0];
        // Move the last node value to the root position
        printf("\nLENGTH: %u\n", *length);
        arr[0] = arr[--*length];
        printf("\nLENGTH: %u\n", *length);
        // Fix a heap structure
        heapify_down(arr, 0, *length);
        // Return a value removed
        return removed;
    }
    // Return 0 in other cases
    return 0;
}

void insert_to_max_heap(unsigned long arr[], unsigned *length, unsigned long value) {
    // Add a value on the 'length' index of an array
    arr[*length] = value;
    // Fix a heap structure after performing an insertion
    heapify_up(arr, *length, 0);
    // Increase a length of an array
    *length += 1;
}

void heapify_down(unsigned long arr[], unsigned curr_idx, unsigned end_idx) {
    // Loop till the current node has a child larger than itself
    // We assume that when we enter a node which both children are
    // smaller than this node, a subtree which a current node is a
    // root of must fulfill a max-heap property.
    while (1) {
        unsigned i, j, k;
        k = curr_idx; // Store an index of a max node index
        i = left_child(k);
        j = right_child(k);

        if (i < end_idx) {
            if (arr[i] > arr[k]) k = i;
            if (j < end_idx && arr[j] > arr[k]) k = j;
        }

        if (k == curr_idx) return;
        // Swap the current with the largest child
        swap(&arr[curr_idx], &arr[k]);
        curr_idx = k;
    }
}

void heapify_up(unsigned long arr[], unsigned curr_idx, unsigned end_idx) {
    while (curr_idx > end_idx) {
        unsigned parent_idx = parent(curr_idx);
        if (arr[curr_idx] > arr[parent_idx]) {
            swap(&arr[curr_idx], &arr[parent_idx]);
        }
        curr_idx = parent_idx;
    }
}

void swap(unsigned long *a, unsigned long *b) {
    unsigned long temp = *b;
    *b = *a;
    *a = temp;
}

inline unsigned left_child(unsigned curr_idx) {
    return curr_idx * 2 + 1;
}

inline unsigned right_child(unsigned curr_idx) {
    return curr_idx * 2 + 2;
}

inline unsigned parent(unsigned curr_idx) {
    return (curr_idx - 1) / 2;
}