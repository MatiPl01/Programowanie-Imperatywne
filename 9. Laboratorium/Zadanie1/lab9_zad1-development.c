#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLENGTH 50
#define CHARCOUNT 26

typedef unsigned short us;
typedef struct Node {
    us ord;
    us idx;
    struct Node* next;
} Node;
typedef struct Step {
    us o;
    us p;
    struct Step* next;
} Step;

int compare(const void *a_ptr, const void *b_ptr);

void sieve_of_eratosthenes(bool primes[]);
void create_chars_ll(const char s[], us n, Node* chars_head);
void create_begin_step_ll(Node* chars_head, Step* steps_head, us n, const bool primes[]);
void find_best_solution(const char s[], us n, Step* steps_head);
void create_result_string(const char s[], us n, char *result, Step* node);
void print_result(const char result[], us n);

void free_chars_ll(Node *head);
void free_steps_ll(Step *head);

// DELME
void print_ll(Node *head);
void print_arr(us *arr, us length);
void print_steps_ll(Step *steps_head);


int main() {
    // Get input data
    us n;
    char s[MAXLENGTH];
    scanf("%s", s);
    n = (us)strlen(s);

    // Generate primes within a range [0, 50]
    bool primes[MAXLENGTH];
    sieve_of_eratosthenes(primes);

    printf("\nPrimes:\n");
    for (us i = 0; i < MAXLENGTH; i = (us)(i + 1)) {
        printf("Num: %hd, Is prime? %hd\n", i, primes[i]);
    }

    Node  *chars_head = malloc(sizeof(Node));
    create_chars_ll(s, n, chars_head);

//    print_ll(chars_head);
    Step *steps_head = malloc(sizeof(Step));
    steps_head->next = NULL;

    print_ll(chars_head);
    printf("Started creating begin, step pairs list\n");

    create_begin_step_ll(chars_head, steps_head, n, primes);

    printf("\nLinked list created successfully\n");
    printf("Linked list contents:\n");
    print_steps_ll(steps_head);

    find_best_solution(s, n, steps_head);

    char *result = (char*)malloc(sizeof(char) * n);
    create_result_string(s, n, result, steps_head->next);

    print_result(result, n);

    // Release memory
    free_chars_ll(chars_head);
    free_steps_ll(steps_head);
    free(result);

    return 0;
}


void sieve_of_eratosthenes(bool primes[]) {
    memset(primes, true, MAXLENGTH);
    primes[0] = primes[1] = false;

    for (us i = 2; i * i < MAXLENGTH; i = (us)(i + 1)) {
        if (primes[i]) {
            for (us j = (us)(i * i); j < MAXLENGTH; j = (us)(j + i)) {
                printf("Setting %hd to be not prime\n", j);
                primes[j] = false;
            }
        }
    }
}


void create_chars_ll(const char s[], us n, Node* chars_head) {
    us ind_arr[CHARCOUNT];
    Node **chars_ind = malloc(sizeof(Node*) * CHARCOUNT);
    Node **tails = malloc(sizeof(Node*) * CHARCOUNT);

    for (us i = 0; i < CHARCOUNT; i = (us)(i + 1)) {
        chars_ind[i] = malloc(sizeof(Node));
        chars_ind[i]->next = NULL;
        ind_arr[i] = USHRT_MAX;
    }

    for (us i = 0; i < CHARCOUNT; i = (us)(i + 1)) tails[i] = chars_ind[i];

    us idx, j = 0;
    for (us i = 0; i < n; i = (us)(i + 1)) {
        idx = (us)(s[i] - 'a');
        printf("Added: %c (idx: %hd) to the array as i=%hd\n", s[i], idx, i);
        Node* node = (Node*)malloc(sizeof(Node));
        node->idx = i;
        node->ord = idx;
        node->next = NULL;
        if (chars_ind[idx]->next == NULL) {
            printf("Adding to ind arr: idx=%hd, j=%hd\n", idx, j);
            ind_arr[j] = idx;
            j = (us)(j + 1);
        }
        tails[idx]->next = node;
        tails[idx] = node;
    }

    qsort(ind_arr, CHARCOUNT, sizeof(us), compare);

    // Merge all linked lists and link to the chars_head
    Node *tail = chars_head;
    us i = 0;
    while (ind_arr[i] < USHRT_MAX && i < CHARCOUNT) {
        idx = ind_arr[i];
        tail->next = chars_ind[idx]->next;
        tail = tails[idx];
        i = (us)(i + 1);
    }
    tail->next = NULL;

    // Release memory
    for (i = 0; i < CHARCOUNT; i = (us)(i + 1)) free(chars_ind[i]);
    free(chars_ind);
    free(tails);
}


void create_begin_step_ll(Node* chars_head, Step* steps_head, us n, const bool primes[]) {
    // Store all (o - begin, p - step) pairs of characters which are lexicographically
    // lowest and their next character (s[(o + p) % n]) is also the lowest
    Node *first, *second;
    us first_best, second_best, diff;
    first_best  = chars_head->next->ord;
    second_best = CHARCOUNT;
    Step *tail  = steps_head;

    first = chars_head->next;
    while (first != NULL) {
        printf("First: (%c, %hd)\n", 'a' + first->ord, first->idx);
        if (first->ord > first_best) break;

        second = chars_head->next;
        while (second != NULL) {
            printf("Second: (%c, %hd)\n", 'a' + second->ord, second->idx);
            if (second == first) {
                second = second->next;
                continue;
            }
            diff = (us)((second->idx - first->idx + n) % n);

            printf("Comparing: first: (%c, %hd), second: (%c, %hd), diff: %hd\n", 'a' + first->ord, first->idx, 'a' + second->ord, second->idx, diff);
            printf("Is diff prime?: %hd\n", primes[diff]);
            if (second->ord > second_best) break;
            if (primes[diff]) {
                if (second->ord < second_best) {
                    printf("\nNew second best: %hd - removing previous result\n", second->ord);
                    second_best = second->ord;
                    // Remove all saved nodes if found better one
                    free_steps_ll(steps_head->next);
                    tail = steps_head;

                    printf("Steps linked list after clearing:\n");
                    print_steps_ll(steps_head);
                }
                if (second->ord == second_best) {
                    Step *node = (Step*)malloc(sizeof(Step));
                    node->o = first->idx;
                    node->p = diff;
                    node->next = NULL;
                    tail->next = node;
                    tail = node;
                    printf("\nAdded new entry: o: %hd, p: %hd\n", node->o, node->p);
                    printf("Steps linked list after adding new entry:\n");
                    print_steps_ll(steps_head);
                }
            }

            second = second->next;
        }

        first = first->next;
    }
}


void find_best_solution(const char s[], us n, Step* steps_head) {
    us i = 2, best, curr, o, p;
    Step* prev;
    Step* temp;

    printf("\nStarted filtering linked list...\n");

    while (i < n && steps_head->next->next != NULL) {
        printf("Iteration number i=%hd.\n", i);
        printf("Linked list at the beginning of the iteration:\n");
        print_steps_ll(steps_head);

        o = steps_head->next->o;
        p = steps_head->next->p;
        best = (us)(s[(o + i * p) % n] - 'a');

        printf("Initial best: %hd\n", best);

        // Start from the second one node
        prev = steps_head->next;
        while (prev->next != NULL) {
            o = prev->next->o;
            p = prev->next->p;
            curr = (us)(s[(o + i * p) % n] - 'a');
            printf("Current: %hd\n", curr);

            // If current item gives greater ASCII code than the best one
            // remove the current item as it's not worth considering
            if (curr > best) {
                printf("Current worse than best, removing the current one (o=%hd, p=%hd).\n", o, p);
                temp = prev->next;
                prev->next = prev->next->next;
                free(temp);
            // If it has the same code as the best one, move ahead
            } else if (curr == best) {
                printf("Current the same as best, skipping.\n");
                prev = prev->next;
            // If found a better solution than the current best one, remove
            // all the preceding nodes in the linked list
            } else {
                printf("Found new best: %hd. Removing everything before (o=%hd, p=%hd)\n", curr, o, p);
                best = curr;
                // Unlink the previous linked list part
                temp = prev->next;
                prev->next = NULL;
                // Remove unlinked part from the memory
                free_steps_ll(steps_head->next);
                // Link the new part to the head
                steps_head->next = temp;
                // Advance the prev pointer
                prev = temp;
            }
        }

        i = (us)(i + 1);
    }
}


void create_result_string(const char s[], us n, char *result, Step* node) {
    us o = node->o;
    us p = node->p;

    for (us i = 0; i < n; i = (us)(i + 1)) {
        result[i] = s[(o + i * p) % n];
    }
}


void print_result(const char result[], us n) {
    for (us i = 0; i < n; i = (us)(i + 1)) {
        printf("%c", result[i]);
    }
}


int compare(const void *a_ptr, const void *b_ptr) {
    int a = *(us*)a_ptr;
    int b = *(us*)b_ptr;
    return a > b ? 1 : -1;
}


void free_chars_ll(Node *head) {
    Node* curr;

    while (head != NULL) {
        curr = head;
        head = head->next;
        free(curr);
    }
}

void free_steps_ll(Step *head) {
    Step* curr;

    while (head != NULL) {
        curr = head;
        head = head->next;
        free(curr);
    }
}


// DELME
void print_ll(Node *head) {
    Node* curr = head->next;

    while (curr != NULL) {
        printf(" -> (%c, %hd)", curr->ord + 'a', curr->idx);
        curr = curr->next;
    }
    printf("\n");
}


void print_arr(us *arr, us length) {
    for (us i = 0; i < length; i = (us)(i + 1)) {
        printf("%hd ", arr[i]);
    }
    printf("\n");
}

void print_steps_ll(Step *steps_head) {
    Step *curr = steps_head->next;

    while (curr != NULL) {
        printf("(o=%hd, p=%hd) -> ", curr->o, curr->p);
        curr = curr->next;
    }
    printf("\n");
}
// END DELME
