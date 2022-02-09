#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>


#define DELTA 50
#define MAXSIZE 10000

typedef unsigned short us;
typedef unsigned long ul;

typedef struct Node {
    us idx;
    struct Node* prev;
    struct Node* next;
} Node;

void read_init_config(ul *board, us n, us delta, Node *head, Node *tail);
void unload_towers(ul *board, Node *head, Node *tail);
void update_towers_ll(const ul *board, Node *curr, Node *head, Node *tail);
void move_counters(ul *board, us size);

void unload_field(ul *board, us idx);
void jump_right(ul *board, us idx);
void jump_left(ul *board, us idx);

void print_results(const ul *board, us size, us delta);

void insert_node_after(Node *node, us idx);


void print_arr(ul *arr, us begin_idx, us end_idx);
void print_ll(Node *head, Node *tail);


int main() {
    // Calculate the size of an array which must be allocated
    us size = (us)(2 * DELTA + MAXSIZE);

    // Allocate an array and get input data
    us n;
    ul *board = (ul*)calloc(size, sizeof(ul));
    Node *indices_head = (Node*)malloc(sizeof(Node));
    Node *indices_tail = (Node*)malloc(sizeof(Node));
    scanf("%hd", &n);
    read_init_config(board, n, DELTA, indices_head, indices_tail);

    printf("\nInitial linked list:\n");
    print_ll(indices_head, indices_tail);

    // Phase 1 - unload all the towers (perform operations in such a way
    // that at the end none of the fields has more than 2 towers)
    unload_towers(board, indices_head, indices_tail);

    printf("\nAfter Phase 1:\n");
    print_arr(board, 0, (us)(size - 1));

    // Phase 2 - make such moves that will lead us to the final configuration
    move_counters(board, size);

    printf("\nAfter Phase 2:\n");
    print_arr(board, 0, (us)(size - 1));

    // Phase 3 - read and print the result
    print_results(board, size, DELTA);

    // Release memory
    free(indices_head);
    free(indices_tail);
    free(board);

    return 0;
}

void read_init_config(ul *board, us n, us delta, Node *head, Node *tail) {
    us p, idx;
    ul m;
    Node *curr = head;

    for (us i = 0; i < n; i = (us)(i + 1)) {
        scanf("%hd %ld", &p, &m);
        idx = (us)(delta + p);
        board[idx] = m;
        if (m > 2) {
            Node *node = (Node*)malloc(sizeof(Node));
            node->idx = idx;
            node->prev = curr;
            curr->next = node;
            curr = node;
        }
    }

    head->prev = NULL;
    curr->next = tail;
    tail->prev = curr;
    tail->next = NULL;
    head->idx = tail->idx = USHRT_MAX;
}

void unload_field(ul *board, us idx) {
    ul delta = board[idx] / 3;
    board[idx] %= 3;
    board[idx - 2] += delta;
    board[idx + 2] += delta;
}

void unload_towers(ul *board, Node *head, Node *tail) {
    Node *curr, *next;
    us i;

    while (head->next != tail) {
        curr = head->next;
        while (curr != tail) {
            i = curr->idx;
            // Unload the tower on the current field
            if (board[i] > 2) {
//                printf("Unloading tower from i=%d (count: %ld)\n", i - DELTA, board[i]);

                // Check if there are already towers on indices i + 2 and i - 2
                // If not, add such indices to the linked list
                next = curr->next;
                unload_field(board, i);
                update_towers_ll(board, curr, head, tail);
                curr = next;

//                printf("Indices linked list:\n");
//                print_ll(head, tail);
            } else {
                curr = curr->next;
            }
        }
    }
}

void update_towers_ll(const ul *board, Node *curr, Node *head, Node *tail) {
    us i = curr->idx;
    bool remove_curr = true;

    if (board[i - 2] > 2) {
        if (curr->prev) {
            if (curr->prev->idx < i - 2 || curr->prev == head) {
                insert_node_after(curr->prev, (us) (i - 2));
            } else if (curr->prev->idx == i - 1) {
                if (curr->prev->prev && curr->prev->prev->idx < i - 2) {
                    insert_node_after(curr->prev->prev, (us) (i - 2));
                }
            }
        } else {
            curr->idx = (us) (i - 2);
            remove_curr = false;
        }
    }

    if (board[i + 2] > 2) {
        if (curr->next) {
            if (curr->next->idx > i + 2 || curr->next == tail) {
                insert_node_after(curr, (us) (i + 2));
            } else if (curr->next->idx == i + 1) {
                if (curr->next->next && curr->next->next->idx > i + 2) {
                    insert_node_after(curr->next, (us) (i + 2));
                }
            }
        } else {
            curr->idx = (us)(i + 2);
            remove_curr = false;
        }
    }

    if (remove_curr) {
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        free(curr);
    }
}

void insert_node_after(Node *node, us idx) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->idx = idx;
    new_node->prev = node;
    new_node->next = node->next;
    node->next->prev = new_node;
    node->next = new_node;
}

void jump_right(ul *board, us idx) {
    board[idx] -= 1;
    board[idx + 1] -= 1;
    board[idx + 2] += 1;
}

void jump_left(ul *board, us idx) {
    board[idx] -= 1;
    board[idx - 1] += 1;
    board[idx - 2] += 1;
}

void move_counters(ul *board, us size) {
    int i = size - 1;

    printf("\n");

    while (i > 0) {
        printf("During Phase 2: (idx = %d)\n", i);
        print_arr(board, (us)(i - 3), (us)(i + 3));

        if (board[i] && board[i + 1]) {
            printf("Case 1 ^\n");
            jump_right(board, (us)i);
            i += 2;
        } else if (board[i] == 2 && board[i - 1]) {
            printf("Case 2 ^\n");
            i--;
        } else if (board[i] == 2 && !board[i + 1]) {
            printf("Case 3 ^\n");
            jump_left(board, (us)i);
            jump_right(board, (us)(i - 1));
            i++;
        } else if (board[i] == 3) {
            printf("Case 4 ^\n");
            unload_field(board, (us)i);
            i += 2;
        } else {
            printf("Case 6 ^\n");
            while (i > 0 && !board[--i]);
        }
    }
}

void print_results(const ul *board, us size, us delta) {
    for (us i = 0; i < size; i = (us)(i + 1)) {
        if (board[i]) printf("%d ", i - delta);
    }
    printf("\n");
}



void print_arr(ul *arr, us begin_idx, us end_idx) {
    for (us i = begin_idx; i <= end_idx; i = (us)(i + 1)) {
        printf("%ld ", arr[i]);
    }
    printf("\n");
}

void print_ll(Node *head, Node *tail) {
    Node *curr = head->next;
    printf("Left to right:\n");
    while (curr != tail) {
        printf("%d ", (int)(curr->idx - DELTA));
        curr = curr->next;
    }
    printf("\nRight to left:\n");
    curr = tail->prev;
    while (curr != head) {
        printf("%d ", (int)(curr->idx - DELTA));
        curr = curr->prev;
    }
    printf("\n");
}
