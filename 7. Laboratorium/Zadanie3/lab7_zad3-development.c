#include <stdio.h>
#include <stdlib.h>


typedef unsigned short us;
typedef struct Rect {
    us begin_idx;
    us end_idx;
    struct Rect* next;
} Rect;
typedef struct Mask {
    us width;
    us height;
    Rect* rows;
    Rect* cols;
} Mask;

void read_values_matrix(int *matrix, us size);
void read_mask_matrix(us *matrix, us num_rows, us num_cols);

void create_mask(Mask *mask, const us *P, us num_rows, us num_cols);
void create_row_rectangles_ll(Rect *result_ll, const us *values_row, us num_cols);
void create_col_rectangles_ll(Rect *result_ll, const us *values_col, us num_cols, us num_rows);
void free_mask(Mask *mask);
void free_rect_ll(Rect *head);

long max_masked_sum(const Mask *mask, const int *matrix, us size);
long calc_init_masked_sum(const Mask *mask, const int *matrix, us size);
long mask_move_right_sum(const Mask *mask, const int *matrix, us size, long curr_sum, us row_idx, us col_idx);
long mask_move_left_sum(const Mask *mask, const int *matrix, us size, long curr_sum, us row_idx, us col_idx);
long mask_move_down_sum(const Mask *mask, const int *matrix, us size, long curr_sum, us row_idx, us col_idx);


// REMOVE ME
void print_rect_ll(Rect *head);


int main() {
    // Prepare variables and read input data
    us n, k, l;
    scanf("%hd %hd %hd", &n, &k, &l);
    int *T = (int*)malloc(sizeof(int) * n * n);
    us *P  = (us*)malloc(sizeof(us) * k * l);

    read_values_matrix((int*)T, n);
    read_mask_matrix((us*)P, k, l);

    // REMOVE ME
    printf("Input matrix of values:\n");
    for (us i = 0; i < n; i = (us)(i + 1)) {
        for (us j = 0; j < n; j = (us)(j + 1)) {
            printf("%i ", *((T + i * n) + j));
        }
        printf("\n");
    }

    printf("\nInput mask matrix:\n");
    for (us i = 0; i < k; i = (us)(i + 1)) {
        for (us j = 0; j < l; j = (us)(j + 1)) {
            printf("%hd ", *((P + i * l) + j));
        }
        printf("\n");
    }

    // Create an object which represents a mask
    Mask *mask = (Mask*)malloc(sizeof(Mask));
    create_mask(mask, (us*)P, k, l);

    // REMOVE ME
    printf("height: %hd, width: %hd\n", mask->height, mask->width);
    printf("Rows rectangles:\n");
    for (us i = 0; i < mask->height; i = (us)(i + 1)) {
        printf("\nRow #%hd:\n", i);
        print_rect_ll(((mask->rows) + i));
    }
    printf("Columns rectangles:\n");
    for (us j = 0; j < mask->width; j = (us)(j + 1)) {
        printf("\nColumn #%hd:\n", j);
        print_rect_ll(((mask->cols) + j));
    }

    printf("\n\n\n");

    // Find max masked sum in the input array of values
    printf("%li", max_masked_sum((Mask*)mask, (int*)T, n));

    // Release memory
    free_mask(mask);
    free(T);
    free(P);

    return 0;
}

void read_values_matrix(int *matrix, us size) {
    for (us i = 0; i < size * size; i = (us)(i + 1)) {
        scanf("%d", (matrix + i));
    }
}

void read_mask_matrix(us *matrix, us num_rows, us num_cols) {
    for (us i = 0; i < num_rows * num_cols; i = (us)(i + 1)) {
        scanf("%hd", (matrix + i));
    }
}

void create_mask(Mask *mask, const us *P, us num_rows, us num_cols) {
    // Create mask object and initialize its properties
    mask->rows = (Rect*)malloc(sizeof(Rect) * num_rows);
    mask->cols = (Rect*)malloc(sizeof(Rect) * num_cols);
    mask->width  = num_cols;
    mask->height = num_rows;

    // Loop over P array of 0-1 values and create rectangles
    // Start by creating horizontal rectangles and adding them to the the proper
    // linked list in a rows array
    for (us i = 0; i < num_rows; i = (us)(i + 1)) {
        Rect *result_ll = (Rect*)malloc(sizeof(Rect));
        result_ll->next = NULL;
        create_row_rectangles_ll(result_ll, (us*)(P + i * num_cols), num_cols);
        *((mask->rows) + i) = *result_ll;
    }

    // Loop again over P matrix and create columns rectangles
    for (us j = 0; j < num_cols; j = (us)(j + 1)) {
        Rect *result_ll = (Rect*)malloc(sizeof(Rect));
        result_ll->next = NULL;
        create_col_rectangles_ll(result_ll, (us*)(P + j), num_cols, num_rows);
        *((mask->cols) + j) = *result_ll;
    }
}

void create_row_rectangles_ll(Rect *result_ll, const us *values_row, us num_cols) {
    Rect *tail = result_ll;

    us begin_idx, end_idx;
    // Add rectangles to the linked list
    for (us j = 0; j < num_cols; j = (us)(j + 1)) {
        if (*(values_row + j) == 1) {
            begin_idx = j;
            while (j < num_cols && *(values_row + j) == 1) {
                j = (us)(j + 1);
            }
            end_idx = (us)(j - 1);

            Rect *new_rect = (Rect*)malloc(sizeof(Rect));
            new_rect->begin_idx = begin_idx;
            new_rect->end_idx = end_idx;
            tail->next = new_rect;
            tail = new_rect;

            printf("Created row rectangle: %hd - %hd\n", begin_idx, end_idx);
        }
    }

    // Point the next pointer of a tail to NULL
    tail->next = NULL;
}

void create_col_rectangles_ll(Rect *result_ll, const us *values_col, us num_cols, us num_rows) {
    Rect *tail = result_ll;

    us begin_idx, end_idx;
    // Add rectangles to the linked list
    for (us i = 0; i < num_rows; i = (us)(i + 1)) {
        if (*(values_col + i * num_cols) == 1) {
            begin_idx = i;
            while (i < num_rows && *(values_col + i * num_cols) == 1) {
                i = (us)(i + 1);
            }
            end_idx = (us)(i - 1);

            Rect *new_rect = (Rect*)malloc(sizeof(Rect));
            new_rect->begin_idx = begin_idx;
            new_rect->end_idx = end_idx;
            tail->next = new_rect;
            tail = new_rect;

            printf("Created col rectangle: %hd - %hd\n", begin_idx, end_idx);
        }
    }

    // Point the next pointer of a tail to NULL
    tail->next = NULL;
}

void free_mask(Mask *mask) {
    printf("\n\n");
    // Remove rows of a mask
    for (us i = 0; i < mask->height; i = (us)(i + 1)) {
        printf("Removing mask row: %hd\n", i);
        free_rect_ll(((mask->rows) + i)->next);   // IMPORTANT !!!!!  WE CANNOT PASS A POINTER TO THE FIRST NODE
    }                                                  // AS IT IS ALSO A PART OF THE mask->rows DYNAMIC ARRAY AND
    free(mask->rows);                                  // REMOVING THIS POINTER WILL RESULT IN LOSING THE CONTINUITY OF
                                                       // THE ARRAY (THERE WILL BE A HOLE IN A CONTINUOUS MEMORY SPACE
                                                       // ALLOCATED BY THE ARRAY SO WE WON'T BE ABLE TO ACCESS ELEMENTS
                                                       // STORED LATER AND REMOVE THEM PROPERLY !!!!!!!!!!!!!!!!!!!!!
                                                       // As a result, we have to leave sentinel nodes of linked lists
                                                       // and remove all nodes linked to them. In the last step we will
                                                       // remove sentinel nodes (they all will be removed when freeing
                                                       // the whole allocated array of Rect* pointers mask->rows) !!!!!
    // Remove columns of a mask
    for (us j = 0; j < mask->width; j = (us)(j + 1)) {
        printf("Removing mask column: %hd\n", j);
        free_rect_ll(((mask->cols) + j)->next);
    }
    free(mask->cols);

    // Remove the mask object itself
    free(mask);
}

void free_rect_ll(Rect *head) {
    Rect* tmp;

    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

long max_masked_sum(const Mask *mask, const int *matrix, us size) {
    // We will move a mask using zig-zag approach (right, down, left, down, right, down, ...)
    // in order to get the maximum sum of values covered by the mask of ones.
    // As we don't have to recalculate the whole sum again, because in most cases there
    // will be only some values dropped and some others taken to the sum, we will subtract
    // values of such numbers which were dropped and add values of numbers which got included.
    long curr_sum, max_sum;
    max_sum = curr_sum = calc_init_masked_sum(mask, matrix, size);
    short j = 0;

    printf("Initial sum of values: %li\n", curr_sum);

    for (us i = 0; i < (us)(size - mask->height + 1); i = (us)(i + 1)) {
        // Move right
        if (i % 2 == 0) {
            for (; j < (short)(size - mask->width); j++) {
                printf("Move right: (%hd, %hd) -> (%hd, %hd)\n", i, j, i, (short)(j + 1));
                curr_sum = mask_move_right_sum(mask, matrix, size, curr_sum, i, (us)j);
                if (curr_sum > max_sum) max_sum = curr_sum;
                printf("\nAfter moving: curr_sum=%li, max_sum=%li\n", curr_sum, max_sum);
            }
        // Move left
        } else {
            for (; j > 0; j--) {
                printf("Move left:  (%hd, %hd) -> (%hd, %hd)\n", i, j, i, (short)(j - 1));
                curr_sum = mask_move_left_sum(mask, matrix, size, curr_sum, i, (us)j);
                if (curr_sum > max_sum) max_sum = curr_sum;
                printf("\nAfter moving: curr_sum=%li, max_sum=%li\n", curr_sum, max_sum);
            }
        }
        // Move down
        if (i < (us)(size - mask->height) || (j > 0 && j < (us)(size - mask->width))) {
            printf("Move down:  (%hd, %hd) -> (%hd, %hd)\n", i, j, (us)(i + 1), j);
            curr_sum = mask_move_down_sum(mask, matrix, size, curr_sum, i, (us)j);
            if (curr_sum > max_sum) max_sum = curr_sum;
            printf("\nAfter moving: curr_sum=%li, max_sum=%li\n", curr_sum, max_sum);
        }
    }

    return max_sum;
}

long calc_init_masked_sum(const Mask *mask, const int *matrix, us size) {
    long result = 0;
    Rect *rect;

    // Iterate over masked rows (could also iterate by columns - it doesn't matter)
    // and store a sum of all masked values
    for (us i = 0; i < mask->height; i = (us)(i + 1)) {
        rect = (mask->rows + i)->next;
        // Loop over all masking rectangles in a row and add all covered values to the result
        while (rect != NULL) {
            for (us j = rect->begin_idx; j <= rect->end_idx; j = (us)(j + 1)) {
                result += *((matrix + i * size) + j);
            }
            rect = rect->next;
        }
    }

    return result;
}

long mask_move_right_sum(const Mask *mask, const int *matrix, us size, long curr_sum, us row_idx, us col_idx) {
    Rect *rect;

    printf("\nProcessing position (right move): %hd %hd\n", row_idx, col_idx);

    for (us i = 0; i < mask->height; i = (us)(i + 1)) {
        rect = (mask->rows + i)->next;
        // Loop over all masking rectangles in a row and correct the current sum
        while (rect != NULL) {
            printf("Moving rectangle: (%d - %d) -> (%d - %d)\n", col_idx + rect->begin_idx, col_idx + rect->end_idx, col_idx + rect->begin_idx + 1, col_idx + rect->end_idx + 1);
            curr_sum -= *((matrix + (row_idx + i) * size) + (col_idx + rect->begin_idx));
            curr_sum += *((matrix + (row_idx + i) * size) + (col_idx + rect->end_idx + 1));
            rect = rect->next;
        }
    }

    return curr_sum;
}

long mask_move_left_sum(const Mask *mask, const int *matrix, us size, long curr_sum, us row_idx, us col_idx) {
    Rect *rect;

    printf("\nProcessing position (left move): %hd %hd\n", row_idx, col_idx);

    for (us i = 0; i < mask->height; i = (us)(i + 1)) {
        rect = (mask->rows + i)->next;
        // Loop over all masking rectangles in a row and correct the current sum
        while (rect != NULL) {
            printf("Moving rectangle: (%d - %d) -> (%d - %d)\n", col_idx + rect->begin_idx, col_idx + rect->end_idx, col_idx + rect->begin_idx - 1, col_idx + rect->end_idx - 1);
            curr_sum -= *((matrix + (row_idx + i) * size) + (col_idx + rect->end_idx));
            curr_sum += *((matrix + (row_idx + i) * size) + (col_idx + rect->begin_idx - 1));
            rect = rect->next;
        }
    }

    return curr_sum;
}

long mask_move_down_sum(const Mask *mask, const int *matrix, us size, long curr_sum, us row_idx, us col_idx) {
    Rect *rect;

    printf("\nProcessing position (down move): %hd %hd\n", row_idx, col_idx);

    for (us j = 0; j < mask->width; j = (us)(j + 1)) {
        rect = (mask->cols + j)->next;
        // Loop over all masking rectangles in a column and correct the current sum
        while (rect != NULL) {
            printf("Moving rectangle: (%d - %d) -> (%d - %d)\n", row_idx + rect->begin_idx, row_idx + rect->end_idx, row_idx + rect->begin_idx + 1, row_idx + rect->end_idx + 1);
            curr_sum -= *((matrix + (row_idx + rect->begin_idx) * size) + (col_idx + j));
            curr_sum += *((matrix + (row_idx + rect->end_idx + 1) * size) + (col_idx + j));
            rect = rect->next;
        }
    }

    return curr_sum;
}

// REMOVE ME
void print_rect_ll(Rect *head) {
    Rect *curr = head->next;
    unsigned i = 1;

    while (curr != NULL) {
        printf("Rectangle #%u:  %hd - %hd\n", i, curr->begin_idx, curr->end_idx);
        curr = curr->next;
        i++;
    }
}
