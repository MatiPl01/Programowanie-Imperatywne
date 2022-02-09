#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


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
void insert_node_after(Node *node, us idx);

void move_counters(ul *board, us size);

void unload_field(ul *board, us idx);
void jump_right(ul *board, us idx);
void jump_left(ul *board, us idx);

void print_results(const ul *board, us size, us delta);


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

    // Phase 1 - unload all the towers (perform operations in such a way
    // that at the end none of the fields has more than 2 towers)
    unload_towers(board, indices_head, indices_tail);

    // Phase 2 - make such moves that will lead us to the final configuration
    move_counters(board, size);

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
        // If a number of counters on a field is at least 3, add this index to the
        // linked list in order to unload towers later without checking all the
        // fields in a board array
        if (m > 2) {
            Node *node = (Node*)malloc(sizeof(Node));
            node->idx = idx;
            node->prev = curr;
            curr->next = node;
            curr = node;
        }
    }

    // Update pointers of the head node and the tail node
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
                next = curr->next;
                unload_field(board, i);
                // Check if there are already towers on indices i + 2 and i - 2
                // If not, add such indices to the linked list
                update_towers_ll(board, curr, head, tail);
                curr = next;
            } else {
                curr = curr->next;
            }
        }
    }
}

void update_towers_ll(const ul *board, Node *curr, Node *head, Node *tail) {
    us i = curr->idx;

    // If a field i-2 has at least 3 towers, check if we have to add an index
    // of this field to the linked list
    if (board[i - 2] > 2) {
        // If the previous node of the current node stores an index lower than i-2
        // or the previous node is a head sentinel node, there is no node with
        // i-2 index stored so we have to insert such a node before the current node
        if (curr->prev->idx < i - 2 || curr->prev == head) {
            insert_node_after(curr->prev, (us)(i - 2));
        // Otherwise, if the previous node stores i-1 index and the node previous to the
        // previous one stores an index lower than i-2 or is a head node, we have to insert
        // a new node before the previous node
        } else if (curr->prev->idx == i - 1 && (curr->prev->prev->idx < i - 2 || curr->prev->prev == head)) {
            insert_node_after(curr->prev->prev, (us)(i - 2));
        }
    }

    // If a field i+2 has at least 3 towers, check if we have to add an index
    // of this field to the linked list
    if (board[i + 2] > 2) {
        // If the next node stores an index greater than i+2 or is a tail node, there is
        // no node with an index i+2 in a linked list, so we have to insert such a node
        if (curr->next->idx > i + 2 || curr->next == tail) {
            insert_node_after(curr, (us)(i + 2));
        // Otherwise, if the next node stores i+2 index and the next node to the next one
        // stores and index greater than i+2 or the next node to the next one is a tail node,
        // there is no node with an index i+2 in a linked list, so we have to insert such a node
        } else if (curr->next->idx == i + 1 && (curr->next->next->idx > i + 2 || curr->next->next == tail)) {
            insert_node_after(curr->next, (us)(i + 2));
        }
    }

    // Remove the current node as a number of counters on a field which it points
    // to was decreased below 3
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    free(curr);
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

    while (i > 0) {
        if (board[i] && board[i + 1]) {
            jump_right(board, (us)i);
            i += 2;
        } else if (board[i] == 2 && board[i - 1]) {
            i--;
        } else if (board[i] == 2 && !board[i + 1]) {
            jump_left(board, (us)i);
            jump_right(board, (us)(i - 1));
            i++;
        } else if (board[i] == 3) {
            unload_field(board, (us)i);
            i += 2;
        } else {
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
