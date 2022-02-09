#include <stdio.h>
#include <stdlib.h>


#define ARR_SIZE 100

typedef unsigned short us;
typedef struct Node {
    int val;
    struct Node* next;
} Node;


Node* build_linked_list(const int arr[], us size);
void build_linked_lists(int *matrix, us size, Node *ll_arr[]);
void free_ll(Node *head);

void print_array(int arr[], us size);
void read_matrix(int *matrix, us size);

Node* merge_two_sorted_lls(Node *ll1_head, Node *ll2_head);
Node* merge_sorted_lls(Node *ll_arr[], us size);
us rewrite_ll_to_arr(Node *head, int target[]);


int main() {
    // Prepare data
    us N;
    scanf("%hd", &N);
    int t1[ARR_SIZE][ARR_SIZE];
    struct Node* ll_arr[ARR_SIZE];
    int t2[ARR_SIZE * ARR_SIZE];

    // Read matrix values
    read_matrix((int*)t1, N);

    // Build linked lists of each row
    build_linked_lists((int*)t1, N, ll_arr);

    // Merge linked list to the one which is sorted and has no repeated values
    Node *result_head = merge_sorted_lls(ll_arr, N);

    // Rewrite values of lined list to the t2 array
    us values_count = rewrite_ll_to_arr(result_head, t2);

    // Print a result array
    print_array(t2, values_count);

    // Free memory (remove linked lists)
    free_ll(ll_arr[0]);

    return 0;
}


void read_matrix(int *matrix, us size) {
    for (us i = 0; i < size; i++) {
        for (us j = 0; j < size; j++) {
            scanf("%d", ((matrix + i * size) + j));
        }
    }
}

void print_array(int arr[], us size) {
    for (us i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

Node* build_linked_list(const int arr[], us size) {
    // Create a sentinel node
    Node *head = (Node*)malloc(sizeof(Node));
    Node *tail = head;

    // Add subsequent values as new nodes
    for (us i = 0; i < size; i++) {
        Node *new_node = (Node*)malloc(sizeof(Node));
        new_node->val = arr[i];
        new_node->next = NULL;
        tail->next = new_node;
        tail = tail->next;
    }

    // Return a pointer to the beginning of a linked list
    return head;
}

void build_linked_lists(int *matrix, us size, Node *ll_arr[]) {
    for (us i = 0; i < size; i++) {
        ll_arr[i] = build_linked_list((matrix + i * size), size);
    }
}

void free_ll(Node *head) {
    Node* tmp;

    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

Node* merge_two_sorted_lls(Node *ll1_head, Node *ll2_head) {
    Node *res_head = (Node*)malloc(sizeof(Node));
    Node *res_tail = res_head;

    Node *ll1_curr = ll1_head->next;
    Node *ll2_curr = ll2_head->next;

    // Merge two lists till both haven't been exhausted
    while (ll1_curr != NULL && ll2_curr != NULL) {
        // If a value of the first list's current node is lower than a value
        // of the second list's current node, link a current node of the first
        // linked list to the result linked list.
        if (ll1_curr->val < ll2_curr->val) {
            int curr_val = ll1_curr->val;
            res_tail->next = ll1_curr;
            ll1_curr = ll1_curr->next;
            // Skip repeated values within a current linked list
            while (ll1_curr != NULL && ll1_curr->val == curr_val) ll1_curr = ll1_curr->next;
            // If a value of the second list's current node is lower than a value
            // of the first list's current node, link a current node of the second
            // linked list to the result linked list.
        } else if (ll1_curr->val > ll2_curr->val) {
            int curr_val = ll2_curr->val;
            res_tail->next = ll2_curr;
            ll2_curr = ll2_curr->next;
            // Skip repeated values within a current linked list
            while (ll2_curr != NULL && ll2_curr->val == curr_val) ll2_curr = ll2_curr->next;
            // Otherwise, link one of two nodes of equal values and skip all the nodes
            // which have repeated values
        } else {
            res_tail->next = ll1_curr;
            int curr_val = ll1_curr->val;
            while (ll1_curr != NULL && ll1_curr->val == curr_val) ll1_curr = ll1_curr->next;
            while (ll2_curr != NULL && ll2_curr->val == curr_val) ll2_curr = ll2_curr->next;
        }
        // Advance a tail pointer`
        res_tail = res_tail->next;
    }

    // Link the remaining values of a linked list which hasn't been exhausted yet.
    while (ll1_curr != NULL) {
        int curr_val = ll1_curr->val;
        // Link remaining values
        res_tail->next = ll1_curr;
        res_tail = ll1_curr;
        ll1_curr = ll1_curr->next;
        // Skip repeated values within a current linked list
        while (ll1_curr != NULL && ll1_curr->val == curr_val) ll1_curr = ll1_curr->next;
    }
    while (ll2_curr != NULL) {
        int curr_val = ll2_curr->val;
        // Link remaining values
        res_tail->next = ll2_curr;
        res_tail = ll2_curr;
        ll2_curr = ll2_curr->next;
        // Skip repeated values within a current linked list
        while (ll2_curr != NULL && ll2_curr->val == curr_val) ll2_curr = ll2_curr->next;
    }

    // Set a tail's next pointer to NULL
    res_tail->next = NULL;

    // Return a pointer to the head (sentinel node) of a result linked list
    return res_head;
}

Node* merge_sorted_lls(Node *ll_arr[], us size) {
    us interval = 1;

    while (interval < size) {
        for (us i = 0; i < size - interval; i = (us)(i + interval * 2)) {
            ll_arr[i] = merge_two_sorted_lls(ll_arr[i], ll_arr[i + interval]);
        }
        interval = (us)(interval * 2);
    }

    return ll_arr[0];
}

us rewrite_ll_to_arr(Node *head, int target[]) {
    Node *curr = head->next;
    us i = 0;

    while (curr != NULL) {
        target[i++] = curr->val;
        curr = curr->next;
    }

    return i;
}
