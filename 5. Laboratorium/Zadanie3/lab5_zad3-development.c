#include <stdio.h>
#include <stdlib.h>


#define ARR_SIZE 100

typedef unsigned short us;
typedef struct Coords {
    us row_idx;
    us col_idx;
    struct Coords* next;
} Coords;

void read_matrix(us *matrix, us size);
unsigned count_squares(us *matrix, Coords **coords_head, us size, unsigned k);
unsigned sq_perimeter_sum(us *matrix, us size, us mid_row, us mid_col, us side_length);

Coords* append_to_ll(Coords *tail, us row_idx, us col_idx);
void print_ll(Coords *head);
void free_ll(Coords *head);


void print_matrix(const us *matrix, us size);


int main() {
    // Get input data
    us n;
    unsigned k;
    us T[ARR_SIZE][ARR_SIZE];
    Coords *coords_head = (Coords*)malloc(sizeof(Coords));
    coords_head->next = NULL;
    scanf("%hd %u", &n, &k);

    // Read values of matrix
    read_matrix((us*)T, n);

    // Print an array which was filled
    print_matrix((us*)T, n);

    // Count squares which have sides of odd length and values placed
    // on their perimeter sum up to k
    printf("%u", count_squares((us*)T, &coords_head, n, k));

    // Print coordinates of squares center points
    print_ll(coords_head);

    // Release memory (empty linked list)
    free_ll(coords_head);

    return 0;
}


void read_matrix(us *matrix, us size) {
    for (us i = 0; i < size; i++) {
        for (us j = 0; j < size; j++) {
            scanf("%hd", ((matrix + i * size) + j));
        }
    }
}

unsigned count_squares(us *matrix, Coords **coords_head, us size, unsigned k) {
    unsigned count = 0;
//    Coords *coords_head2 = (Coords*)malloc(sizeof(Coords));
//    coords_head2->next = NULL;
    Coords* coords_ll_tail = *coords_head;

    for (us side_length = 3; side_length <= size; side_length = (us)(side_length + 2)) {
        us mid_idx = (us)(side_length / 2);
        for (us i = mid_idx; i < size - mid_idx; i++) {
            for (us j = mid_idx; j < size - mid_idx; j++) {
                printf("\ni %hd j %hd\n", i, j);
                if (sq_perimeter_sum(matrix, size, i, j, side_length) == k) {
                    count++;
                    printf("\n\nAdded new value (coordinates: %hd, %hd)\n\n", i, j);
                    coords_ll_tail = append_to_ll(coords_ll_tail, i, j);
//                    Coords* new_node = malloc(sizeof(Coords));
//                    printf("\nNew node created? %d", new_node != NULL);
//                    new_node->next = NULL;
//                    new_node->row_idx = i;
//                    new_node->col_idx = j;
//                    coords_ll_tail->next = new_node;
//                    coords_ll_tail = coords_ll_tail->next;
                }
            }
        }
    }

    return count;
}

unsigned sq_perimeter_sum(us *matrix, us size, us mid_row, us mid_col, us side_length) {
    unsigned sum = 0;

    us half_length = side_length / 2;
    us begin_row = (us)(mid_row - half_length);
    us begin_col = (us)(mid_col - half_length);

    // Sum values on horizontal sides
    for (us j = begin_col; j < begin_col + side_length; j++) {
        printf("\nSumming from coords: i %hd j %hd val %hd", begin_row, j, *((matrix + size * begin_row) + j));
        sum += *((matrix + size * begin_row) + j);
        printf("\nSumming from coords: i %hd j %hd val %hd", (us)(begin_row + side_length - 1), j, *((matrix + size * (begin_row + side_length - 1)) + j));
        sum += *((matrix + size * (begin_row + side_length - 1)) + j);
    }
    printf("\nSum1: %u", sum);

    // Sum values on vertical sides (without corners which were added above)
    for (us i = (us)(begin_row + 1); i < begin_row + side_length - 1; i++) {
        sum += *((matrix + size * i) + begin_col);
        sum += *((matrix + size * i) + (begin_col + side_length - 1));
    }

    printf("\nSum2: %u", sum);

    return sum;
}

Coords* append_to_ll(Coords *tail, us row_idx, us col_idx) {
    printf("\nStarted appending...");
    Coords* new_node = (Coords*)malloc(sizeof(Coords));
    new_node->next = NULL;
    new_node->row_idx = row_idx;
    new_node->col_idx = col_idx;
    printf("\nCreated new node...");
    printf("Tail node: %hd %hd", tail->row_idx, tail->col_idx);
    tail->next = new_node;
    printf("\nNew tail: row %hd, col %hd", new_node->row_idx, new_node->col_idx);
    return new_node;
}

void print_ll(Coords *head) {
    printf("\n\nPrinting linked list:");
    Coords* curr = head->next;
    while (curr != NULL) {
        printf("\n%hd %hd", curr->row_idx, curr->col_idx);
        curr = curr->next;
    }
}

void free_ll(Coords *head) {
    Coords* tmp;

    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void print_matrix(const us *matrix, us size) {
    for (us i = 0; i < size; i++) {
        for (us j = 0; j < size; j++) {
            printf("%hd ", *((matrix + i * size) + j));
        }
        printf("\n");
    }
}