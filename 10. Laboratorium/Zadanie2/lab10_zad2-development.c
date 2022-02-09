#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define MAXSIZE 10000


typedef unsigned short us;

void read_init_config(long *board, us n, us delta);
void unload_towers(long *board, us size, us delta);
void move_counters(long *board, us size);

inline void unload_field(long *board, us idx);
inline void jump_right(long *board, us idx);
inline void jump_left(long *board, us idx);

void print_results(const long *board, us size, us delta);

void print_arr(long *arr, us begin_idx, us end_idx);


int main() {
    // Calculate the size of an array which must be allocated
    us delta = 50;
    us size = (us)(2 * delta + MAXSIZE);

    // Allocate an array and get input data
    us n;
    long *board = (long*)calloc(size, sizeof(long));
    scanf("%hd", &n);
    read_init_config(board, n, delta);

    printf("\nInput array:\n");
    print_arr(board, (us)(delta - 4), (us)(delta + 6));

    // Phase 1 - unload all the towers (perform operations in such a way
    // that at the end none of the fields has more than 2 towers)
    unload_towers(board, size, delta);

    printf("\nAfter Phase 1:\n");
    print_arr(board, 0, (us)(size - 1));

    // Phase 2 - make such moves that will lead us to the final configuration
    move_counters(board, size);

    printf("\nAfter Phase 2:\n");
    print_arr(board, 0, (us)(size - 1));

    // Phase 3 - read and print the result
    print_results(board, size, delta);

    // Release memory
    free(board);

    return 0;
}


void read_init_config(long *board, us n, us delta) {
    us p;
    long m;
    for (us i = 0; i < n; i = (us)(i + 1)) {
        scanf("%hd %ld", &p, &m);
        board[delta + p] = m;
    }
}

inline void unload_field(long *board, us idx) {
    board[idx] -= 3;
    board[idx - 2] += 1;
    board[idx + 2] += 1;
}

void unload_towers(long *board, us size, us delta) {
    bool found = true;
    us first_idx = delta;
    us last_idx, temp;
    last_idx = temp = (us)(size - delta);

    while (found) {
        found = false;
        for (us i = first_idx; i <= last_idx; i = (us)(i + 1)) {
            // Unload the tower on the current field
            if (board[i] > 2) {
//                printf("Unloading tower from i=%hd (count: %ld)\n", i, board[i]);
                unload_field(board, i);
                // Check if there will be a new tower before
                if (!found && board[i - 2] > 2) {
                    found = true;
                    first_idx = (us)(i - 2);
                } else if (board[i + 2] > 2) {
                    found = true;
                    temp = (us)(i + 2);
                } else if (board[i] > 2) {
                    found = true;
                    temp = i;
                }
            }
        }
        last_idx = temp;
    }
}

inline void jump_right(long *board, us idx) {
    board[idx] -= 1;
    board[idx + 1] -= 1;
    board[idx + 2] += 1;
}

inline void jump_left(long *board, us idx) {
    board[idx] -= 1;
    board[idx - 1] += 1;
    board[idx - 2] += 1;
}

void move_counters(long *board, us size) {
    int i = size - 1;

    printf("\n");

    while (i > 0) {
        printf("During Phase 2: (idx = %d)\n", i);
        if (board[i] && board[i + 1]) {
            printf("Case 1 vvv\n");
            jump_right(board, (us)i);
            i += 2;
        } else if (board[i] == 2 && board[i - 1]) {
            printf("Case 2 vvv\n");
            i--;
        } else if (board[i] == 2 && !board[i + 1]) {
            printf("Case 3 vvv\n");
            jump_left(board, (us)i);
            jump_right(board, (us)(i - 1));
            i++;
        } else if (board[i] == 3) {
            printf("Case 4 vvv\n");
            unload_field(board, (us)i);
            i += 2;
        } else {
            printf("Case 6 vvv\n");
            while (i > 0 && !board[--i]);
        }
    }
}

void print_results(const long *board, us size, us delta) {
    for (us i = 0; i < size; i = (us)(i + 1)) {
        if (board[i]) printf("%d ", i - delta);
    }
    printf("\n");
}


void print_arr(long *arr, us begin_idx, us end_idx) {
    for (us i = begin_idx; i <= end_idx; i = (us)(i + 1)) {
        printf("%ld ", arr[i]);
    }
    printf("\n");
}
