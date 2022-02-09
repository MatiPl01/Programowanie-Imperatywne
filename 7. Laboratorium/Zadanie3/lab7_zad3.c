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


int main() {
    // Prepare variables and read input data
    us n, k, l;
    scanf("%hd %hd %hd", &n, &k, &l);
    int *T = (int*)malloc(sizeof(int) * n * n);
    us *P  = (us*)malloc(sizeof(us) * k * l);

    read_values_matrix((int*)T, n);
    read_mask_matrix((us*)P, k, l);

    // Create an object which represents a mask
    Mask *mask = (Mask*)malloc(sizeof(Mask));
    create_mask(mask, (us*)P, k, l);

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
        create_row_rectangles_ll(((mask->rows) + i), (us*)(P + i * num_cols), num_cols);
    }

    // Loop again over P matrix and create columns rectangles
    for (us j = 0; j < num_cols; j = (us)(j + 1)) {
        create_col_rectangles_ll(((mask->cols) + j), (us*)(P + j), num_cols, num_rows);
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
        }
    }

    // Point the next pointer of a tail to NULL
    tail->next = NULL;
}

void free_mask(Mask *mask) {
    // Remove linked lists in an array of mask rows
    for (us i = 0; i < mask->height; i = (us)(i + 1)) {
        // Remove all nodes linked to the first one (except the first one as we don't
        // want to lose it (an array of linked lists will be impossible to remove after
        // removing the first node of a linked list as it is a part of this array)
        free_rect_ll(((mask->rows) + i)->next);
    }
    // Remove an array of linked lists beginnings
    free(mask->rows);

    // Remove linked lists in an array of mask columns
    for (us j = 0; j < mask->width; j = (us)(j + 1)) {
        // Also remove all nodes linked to the array of columns linked lists
        free_rect_ll(((mask->cols) + j)->next);
    }
    // Remove an array of linked lists beginnings
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
    long max_sum, curr_sum;
    max_sum = curr_sum = calc_init_masked_sum(mask, matrix, size);
    short j = 0;

    for (us i = 0; i < (us)(size - mask->height + 1); i = (us)(i + 1)) {
        // Move right
        if (i % 2 == 0) {
            for (; j < (short)(size - mask->width); j++) {
                curr_sum = mask_move_right_sum(mask, matrix, size, curr_sum, i, (us)j);
                if (curr_sum > max_sum) max_sum = curr_sum;
            }
            // Move left
        } else {
            for (; j > 0; j--) {
                curr_sum = mask_move_left_sum(mask, matrix, size, curr_sum, i, (us)j);
                if (curr_sum > max_sum) max_sum = curr_sum;
            }
        }
        // Move down (if still can move down)
        if (i < (us)(size - mask->height) || (j > 0 && j < (us)(size - mask->width))) {
            curr_sum = mask_move_down_sum(mask, matrix, size, curr_sum, i, (us)j);
            if (curr_sum > max_sum) max_sum = curr_sum;
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

    for (us i = 0; i < mask->height; i = (us)(i + 1)) {
        rect = (mask->rows + i)->next;
        // Loop over all masking rectangles in a row and correct the current sum
        while (rect != NULL) {
            curr_sum -= *((matrix + (row_idx + i) * size) + (col_idx + rect->begin_idx));
            curr_sum += *((matrix + (row_idx + i) * size) + (col_idx + rect->end_idx + 1));
            rect = rect->next;
        }
    }

    return curr_sum;
}

long mask_move_left_sum(const Mask *mask, const int *matrix, us size, long curr_sum, us row_idx, us col_idx) {
    Rect *rect;

    for (us i = 0; i < mask->height; i = (us)(i + 1)) {
        rect = (mask->rows + i)->next;
        // Loop over all masking rectangles in a row and correct the current sum
        while (rect != NULL) {
            curr_sum -= *((matrix + (row_idx + i) * size) + (col_idx + rect->end_idx));
            curr_sum += *((matrix + (row_idx + i) * size) + (col_idx + rect->begin_idx - 1));
            rect = rect->next;
        }
    }

    return curr_sum;
}

long mask_move_down_sum(const Mask *mask, const int *matrix, us size, long curr_sum, us row_idx, us col_idx) {
    Rect *rect;

    for (us j = 0; j < mask->width; j = (us)(j + 1)) {
        rect = (mask->cols + j)->next;
        // Loop over all masking rectangles in a column and correct the current sum
        while (rect != NULL) {
            curr_sum -= *((matrix + (row_idx + rect->begin_idx) * size) + (col_idx + j));
            curr_sum += *((matrix + (row_idx + rect->end_idx + 1) * size) + (col_idx + j));
            rect = rect->next;
        }
    }

    return curr_sum;
}
