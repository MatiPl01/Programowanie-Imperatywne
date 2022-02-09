#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define ARR_SIZE 100

typedef unsigned short us;
typedef struct Stack {
    us next_idx;
    us max_size;
    us* arr;
} Stack;

Stack* create_stack(us max_size);
void delete_stack(Stack* stack);
bool is_stack_empty(Stack* stack);
void push_on_stack(Stack* stack, us val);
us pop_from_stack(Stack* stack);
us peek_stack(Stack* stack);

void read_matrix(us *matrix, us size);
us largest_rect_area(const us *matrix, us size);
us largest_rect_in_histogram(const us heights[], us size, Stack* heights_stack, Stack* begins_stack);

inline us max(us a, us b);
inline us min(us a, us b);


int main() {
    // Get input data
    us n;
    us T[ARR_SIZE][ARR_SIZE];
    scanf("%hd", &n);
    read_matrix((us*)T, n);

    // Print a result (largest rectangle area)
    printf("%hd", largest_rect_area((us*)T, n));

    return 0;
}


Stack* create_stack(us max_size) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->max_size = max_size;
    stack->next_idx = 0;
    stack->arr = (us*)malloc(stack->max_size * sizeof(us));
    return stack;
}

void delete_stack(Stack* stack) {
    free(stack->arr);
    free(stack);
}

bool is_stack_empty(Stack* stack) {
    return stack->next_idx == 0;
}

void push_on_stack(Stack* stack, us val) {
    *((stack->arr) + (stack->next_idx)++) = val;
}

us pop_from_stack(Stack* stack) {
    if (!(is_stack_empty(stack)))
        return *((stack->arr) + --(stack->next_idx));
    return 0;
}

us peek_stack(Stack* stack) {
    if (is_stack_empty(stack)) return 0;
    return *((stack->arr) + (stack->next_idx - 1));
}

void read_matrix(us *matrix, us size) {
    for (us i = 0; i < size; i++) {
        for (us j = 0; j < size; j++) {
            scanf("%hd", ((matrix + i * size) + j));
        }
    }
}

us largest_rect_area(const us *matrix, us size) {
    /* We will map each row of an input matrix to the row of largest possible
     * heights of rectangles which bottom side is placed in an 'i' row and 'j'
     * column and which top part lies somewhere in a 'j' column.
     * (In other words, we will create a histogram of heights of rectangles
     * in each column).
     *
     * As we don't have to refer back to the rows which have already been
     * processed, we will create only one helper array which will be modified
     * during the function execution.
     *
     * After having all heights of rectangles of each column, we will use
     * Largest Rectangle in Histogram finding algorithm to look for a largest
     * rectangle in an input matrix.
     */

    /* Look at the example matrix (input matrix on the left, mapped row on the
     * right side - we will store each row temporarily):
     *   0 1 0 1 0           1 0 1 0 1
     *   0 0 0 0 0           2 1 2 1 2
     *   0 0 0 0 1    -->    3 2 3 2 0
     *   1 0 0 0 0           0 3 4 3 1
     *   0 1 0 0 0           1 0 5 4 2
     */

    // Create an array of heights which is larger than number of columns by 1
    // (the last one value will he a sentinel height which will simplify
    // largest_rect_in_histogram execution process)
    us *heights_arr = (us*)calloc((us)(size + 1), sizeof(us));
    us curr_max_area, max_area = 0;

    Stack *heights_stack = create_stack(size);
    Stack *begins_stack  = create_stack(size);

    for (us i = 0; i < size; i++) {
        for (us j = 0; j < size; j++) {
            // Store 0 as a height of a rectangle when we cannot create a rectangle
            if (*((matrix + i * size) + j)) {
                heights_arr[j] = 0;
                // Else, store a height of the previous rectangle increased by one (we
                // can enlarge the rectangle which ended in the previous row)
            } else {
                heights_arr[j] = (us)(heights_arr[j] + 1);
            }
        }

        // Calculate the largest rectangle area which bottom side is placed in the
        // current row. As we do this process for every row, we have to assess whether
        // we found a rectangle of greater area than the previous max.
        curr_max_area = largest_rect_in_histogram(heights_arr, size, heights_stack, begins_stack);
        max_area = max(max_area, curr_max_area);
    }

    // Release memory
    free(heights_arr);
    delete_stack(heights_stack);
    delete_stack(begins_stack);

    return max_area;
}

us largest_rect_in_histogram(const us heights[], us size, Stack* heights_stack, Stack* begins_stack) {
    us max_area = 0, curr_area;
    us last_begin_idx;

    // Loop over all heights of rectangle histogram bars
    for (us i = 0; i < size + 1; i++) {
        last_begin_idx = (us)(size + 1);

        // Keep removing rectangles from a stack while a stack is still not empty and a height
        // of a current rectangle (from 'i' column) is lower than a height of the last rectangle
        // stored on a stack. For each rectangle removed calculate an area (we are sure all the
        // rectangles which are removed have heights greater than the current one rectangle) and
        // compare the area with the current maximum.
        while (!is_stack_empty(heights_stack) && peek_stack(heights_stack) > heights[i]) {
            last_begin_idx = pop_from_stack(begins_stack);
            curr_area = (us)((i - last_begin_idx) * pop_from_stack(heights_stack));
            max_area = max(max_area, curr_area);
        }

        // If there are no more rectangles on a stack or the current rectangle has a height
        // greater than the last one added to the stack, we will push its height and beginning
        // index on a stack. As we can see there will always be rectangles stored in ascending
        // height order.
        if (is_stack_empty(heights_stack) || peek_stack(heights_stack) < heights[i]) {
            push_on_stack(heights_stack, heights[i]);
            push_on_stack(begins_stack, min(last_begin_idx, i));
        }
    }

    return max_area;
}

inline us max(us a, us b) {
    return a > b ? a : b;
}

inline us min(us a, us b) {
    return a < b ? a : b;
}
