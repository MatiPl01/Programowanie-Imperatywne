#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
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


int main() {
    // Get input data
    us n;
    char s[MAXLENGTH];
    scanf("%s", s);
    n = (us)strlen(s);

    // Generate primes within a range [0, 50]
    bool primes[MAXLENGTH];
    sieve_of_eratosthenes(primes);

    // Create a linked list of lexicographically sorted character-index
    // pairs of s string characters
    Node  *chars_head = malloc(sizeof(Node));
    create_chars_ll(s, n, chars_head);

    // Create a linked list of currently best beginning points and steps
    // (o, p) pairs based on first two characters
    Step *steps_head = malloc(sizeof(Step));
    steps_head->next = NULL;
    create_begin_step_ll(chars_head, steps_head, n, primes);

    // Filter out all (o, p) pairs which will give us lexicographically greater
    // string than the lowest one (by comparing subsequent characters of each
    // of the strings we can obtain from such (o, p) pairs)
    find_best_solution(s, n, steps_head);

    // Having filtered all the solution, we ended up with the best (o, p) pair
    // in the linked list, so we can rewrite appropriate s string's characters
    // to the result array
    char *result = (char*)malloc(sizeof(char) * n);
    create_result_string(s, n, result, steps_head->next);

    // Show the result which was created
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
                primes[j] = false;
            }
        }
    }
}


void create_chars_ll(const char s[], us n, Node* chars_head) {
    us ind_arr[CHARCOUNT];
    Node **chars_ind = malloc(sizeof(Node) * CHARCOUNT);
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
        Node* node = (Node*)malloc(sizeof(Node));
        node->idx = i;
        node->ord = idx;
        node->next = NULL;
        if (chars_ind[idx]->next == NULL) {
            ind_arr[j] = idx;
            j = (us)(j + 1);
        }
        tails[idx]->next = node;
        tails[idx] = node;
    }

    // Sort all indices (which correspond to the ASCII codes, e.g. 0 - 'a', 1 - 'b', etc.)
    // from the lowest to the highest.
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
        if (first->ord > first_best) break;

        second = chars_head->next;
        while (second != NULL) {
            if (second == first) {
                second = second->next;
                continue;
            }
            diff = (us)((second->idx - first->idx + n) % n);

            if (second->ord > second_best) break;
            if (primes[diff]) {
                if (second->ord < second_best) {
                    second_best = second->ord;
                    // Remove all saved nodes if found better one
                    free_steps_ll(steps_head->next);
                    tail = steps_head;
                }
                if (second->ord == second_best) {
                    Step *node = (Step*)malloc(sizeof(Step));
                    node->o = first->idx;
                    node->p = diff;
                    node->next = NULL;
                    tail->next = node;
                    tail = node;
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

    while (i < n && steps_head->next->next != NULL) {
        o = steps_head->next->o;
        p = steps_head->next->p;
        best = (us)(s[(o + i * p) % n] - 'a');

        // Start from the second one node
        prev = steps_head->next;
        while (prev->next != NULL) {
            o = prev->next->o;
            p = prev->next->p;

            curr = (us)(s[(o + i * p) % n] - 'a');
            // If current item gives greater ASCII code than the best one
            // remove the current item as it's not worth considering
            if (curr > best) {
                temp = prev->next;
                prev->next = prev->next->next;
                free(temp);
                // If it has the same code as the best one, move ahead
            } else if (curr == best) {
                prev = prev->next;
                // If found a better solution than the current best one, remove
                // all the preceding nodes in the linked list
            } else {
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
