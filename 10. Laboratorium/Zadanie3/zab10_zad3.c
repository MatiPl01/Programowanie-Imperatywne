#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef unsigned short us;
typedef unsigned long ul;
typedef unsigned long long ull;
typedef struct Node {
    us val;
    struct Node* next;
} Node;

void create_graph(Node *G, us n, us m);
void get_neighbours_counts(Node *G, us g_size, us *result);
void topological_sort(Node *G, us n, us m, us *result);
void dfs(Node *G, us u, us n, bool *visited, us *result, us *idx);
void transpose_graph(Node *G, us g_size, const us *neighbours_counts);
void free_neighbours_lists(Node *G, us n);

Node* add_node(Node *tail, us idx);
us ll_length(Node *head);
us ll_popleft(Node *head);
void free_ll(Node *head);

Node* create_polynomial(Node *G, us n, us m, us *neighbours_counts, const us *t_sorted);
Node* get_new_polynomial(Node *p1_ll, Node *p2_ll);

void read_bin_num(short *result, us n);
void subtract_bin(short *a, const short *b, us n);
ull bin_to_dec(const short *bin, us n);

ul count_results(short *a, short *b, us n, Node *polynomial);
us count_ones_next_bin(short *bin, us n, us prev_count, const bool *is_coefficient);


int main() {
    // Get input data
    us n, m, out;
    scanf("%hd %hd %hd", &n, &m, &out);
    us g_size = (us)(n + m + 1);

    // Create a graph representing the net of xor gates
    Node *G = (Node*)malloc(sizeof(Node) * g_size);
    create_graph(G, n, m);

    // Get topologically sorted numbers of gates (only - without sources)
    us *t_sorted = (us*)malloc(sizeof(us) * m);
    topological_sort(G, n, m, t_sorted);

    // Count a number of each vertex neighbours (by neighbours I mean vertices
    // which represent gates which input is connected to the parent's gate output)
    us *neighbours_counts = (us*)calloc(g_size, sizeof(us));
    get_neighbours_counts(G, g_size, neighbours_counts);

    // Transpose a graph in order to bet parents of each gate easily (there must be
    // only two parents for each gate as a gate has two input pins)
    transpose_graph(G, g_size, neighbours_counts);

    // Calculate the polynomial which will represent the relation between input
    // states on each source and the output state in the output (it will help us
    // assess which state (0 or 1) we have in the output based in the input)
    Node *out_polynomial = create_polynomial(G, n, m, neighbours_counts, t_sorted);

    // Release memory (remove useless data structures created before)
    free_neighbours_lists(G, g_size);
    free(neighbours_counts);
    free(t_sorted);
    free(G);

    // Get the remaining input (two binary numbers which represent the lower and
    // the upper bound of binary values in the input)
    short *a = (short*)malloc(sizeof(short) * (n));
    short *b = (short*)malloc(sizeof(short) * (n));
    read_bin_num(a, n);
    read_bin_num(b, n);

    // Calculate how many unique inputs between a nd b (inclusive) are there for
    // which we get 1 state in the output (we will use the array of polynomial
    // coefficients to make fast output calculations)
    printf("%lu", count_results(a, b, n, out_polynomial));

    // Release memory (the polynomial and binary numbers)
    free_ll(out_polynomial);
    free(a);
    free(b);

    return 0;
}


void create_graph(Node *G, us n, us m) {
    short u, v;
    us i, g_size = (us)(n + m + 1);

    // Create an array of pointers to the last element of each neighbours linked
    // list in order to quickly insert new values
    // (We will store sources of the net on the first n indices of the
    // graph array, then will be a one field empty and later m fields for gates)
    Node **tails = malloc(sizeof(Node*) * g_size);
    for (i = 0; i < g_size; i++) {
        tails[i] = &G[i];
        tails[i]->next = NULL;
    }

    // For each gate add its inputs (directed edges) to the graph
    for (i = 1; i <= m; i++) {
        scanf("%hd %hd", &u, &v);
        tails[u + n] = add_node(tails[u + n], (us)(i + n));
        tails[v + n] = add_node(tails[v + n], (us)(i + n));
    }

    // Release memory
    free(tails);
}

void get_neighbours_counts(Node *G, us g_size, us *result) {
    // This function calculates a number of all the neighbours (children)
    // of each vertex in a graph
    for (us i = 0; i < g_size; i++) {
        result[i] = ll_length(&G[i]);
    }
}

void topological_sort(Node *G, us n, us m, us *result) {
    // This function handles topological sorting of the gates part of a graph
    bool *visited = (bool*)calloc((us)(m + 1), sizeof(bool));
    us idx = m;

    // Use a dfs algorithm to traverse a graph and store topologically sorted results
    for (us i = 1; i <= m; i++) {
        if (!visited[i]) dfs(G, i, n, visited, result, &idx);
    }

    // Release memory
    free(visited);
}

void dfs(Node *G, us u, us n, bool *visited, us *result, us *idx) {
    visited[u] = true;
    Node *curr = (G + n + u)->next;

    // Visit all the neighbours (children) of the current node (gate)
    while (curr) {
        if (!visited[curr->val - n]) {
            dfs(G, (us)(curr->val - n), n, visited, result, idx);
        }
        curr = curr->next;
    }

    // Store an index of the current gate in the results array
    *idx = (us)(*idx - 1);
    result[*idx] = u;
}

void transpose_graph(Node *G, us g_size, const us *neighbours_counts) {
    // This function transposes a graph in place (reverses all directed
    // edges in such a way that they point in an opposite direction)

    // Create an array of last nodes of each graph vertex's neighbours
    Node **tails = malloc(sizeof(Node*) * g_size);
    for (us i = 0; i < g_size; i++) {
        tails[i] = &G[i];
        while (tails[i]->next) tails[i] = tails[i]->next;
    }

    // In a loop, remove all old neighbours of each node (a number
    // of neighbours to remove is the same as the initial number of
    // vertex's neighbours) and for each of the u vertex's neighbours
    // add u vertex to their neighbours linked list
    bool removed_tail;
    for (us u = 0; u < g_size; u++) {
        removed_tail = false;
        for (us i = 0; i < neighbours_counts[u]; i++) {
            if ((G + u)->next == tails[u]) removed_tail = true;
            // Remove a vertex (node) from the left side of the neighbours
            // linked list
            us v = ll_popleft(&G[u]);
            // If we removed the last node from a linked list, we have to
            // update our tail pointer as it now points to the nonexistent vertex
            if (removed_tail) tails[u] = &G[u];
            if (!((G + u)->next)) tails[u]->next = NULL;
            // Insert a node on the right side of the neighbours linked list
            // of the vertex which was removed
            tails[v] = add_node(tails[v], u);
        }
    }

    // Release memory
    free(tails);
}

void free_neighbours_lists(Node *G, us n) {
    // This function removes all neighbours linked lists from the graph
    for (us i = 0; i < n; i++) {
        free_ll((&G[i])->next);
    }
}


Node* add_node(Node *tail, us idx) {
    // This function adds a node to the linked list (after the tail node specified)
    // and return a new tail node
    Node *v = (Node*)malloc(sizeof(Node));
    v->val = idx;
    tail->next = v;
    v->next = NULL;
    return v;
}

us ll_length(Node *head) {
    // This function calculates a length of a linked list
    Node *curr = head->next;
    us count = 0;

    while (curr) {
        count++;
        curr = curr->next;
    }

    return count;
}

us ll_popleft(Node *head) {
    // This function removes the first (leftmost) node from the linked list
    // and returns its value
    Node *removed = head->next;
    us v = removed->val;
    head->next = removed->next;
    free(removed);
    return v;
}

void free_ll(Node *head) {
    // This function removes a linked list from memory
    Node *temp;

    while (head) {
        temp = head->next;
        free(head);
        head = temp;
    }
}


Node* create_polynomial(Node *G, us n, us m, us *neighbours_counts, const us *t_sorted) {
    us g_size = (us)(n + m + 1);
    Node *polynomials = malloc(sizeof(Node) * g_size);

    // Initialise polynomials array with sources numbers from source vertices
    // and empty (sentinel) nodes for each gate
    for (us i = 0; i < g_size; i++) {
        if (i < n) add_node(&polynomials[i], (us)(n - i));
        else (polynomials + i)->next = NULL;
    }

    // In a loop, traverse a graph using a topological sort order and calculate
    // a polynomial (its coefficients) for each gate, based on polynomials
    // of its two parents gates
    us u, v, w;
    for (us i = 0; i < m; i++) {
        u = (us)(t_sorted[i] + n);
        // Get indices of gate's parents vertices
        v = (G + u)->next->val;
        w = (G + u)->next->next->val;
        // Calculate a polynomial for the u vertex
        (polynomials + u)->next = get_new_polynomial(&polynomials[v], &polynomials[w]);
        // Decrement a number of neighbours (which will indicate how many times
        // we will refer to the polynomial later)
        neighbours_counts[v]--;
        neighbours_counts[w]--;
        // If this number dropped below 0, there is no point to store a polynomial
        // for vertices which we will never refer to more, so we will remove this
        // polynomial from memory
        if (!neighbours_counts[v]) {
            free_ll((polynomials + v)->next);
            (polynomials + v)->next = NULL;
        }
        if (!neighbours_counts[w]) {
            free_ll((polynomials + w)->next);
            (polynomials + w)->next = NULL;
        }
    }

    // Create a sentinel node of the final polynomial and link a linked list
    // of its coefficients to this node
    Node *head = (Node*)malloc(sizeof(Node));
    head->next = (polynomials + n + t_sorted[m - 1])->next;

    // Release memory
    free(polynomials);

    // Return a head of the final polynomial coefficients linked list
    return head;
}

Node* get_new_polynomial(Node *p1_ll, Node *p2_ll) {
    // This function calculates a new polynomial based on two input
    // polynomials. The idea is simple as we are sure that both lists
    // will be always sorted in a non-decreasing order so we can simply
    // merge this linked lists (or rather create a new linked list which
    // will have all the values from both input linked lists which aren't
    // in both lists at the same time
    Node *head = (Node*)malloc(sizeof(Node));
    Node *tail = head;
    head->next = NULL;
    Node *pt1 = p1_ll->next, *pt2 = p2_ll->next;

    while (pt1 && pt2) {
        if (pt1->val == pt2->val) {
            pt1 = pt1->next;
            pt2 = pt2->next;
        } else if (pt1->val < pt2->val) {
            tail = add_node(tail, pt1->val);
            pt1 = pt1->next;
        } else {
            tail = add_node(tail, pt2->val);
            pt2 = pt2->next;
        }
    }

    while (pt1) {
        tail = add_node(tail, pt1->val);
        pt1 = pt1->next;
    }

    while (pt2) {
        tail = add_node(tail, pt2->val);
        pt2 = pt2->next;
    }

    Node* returned = head->next;
    free(head);
    return returned;
}


void read_bin_num(short *result, us n) {
    // This function gets a binary number from input and converts
    // this number (string) to the array of binary digits
    char *chars = (char*)malloc((us)(n + 1) * sizeof(char));
    scanf("%s", chars);

    for (us i = 0; i < n; i++) result[i] = (short)(chars[i] - '0');

    free(chars);
}

void subtract_bin(short *a, const short *b, us n) {
    // This function handles positive-result binary numbers subtraction
    for (short i = (short)(n - 1); i >= 0; i--) {
        a[i] = (short)(a[i] - b[i]);
        if (a[i] < 0) {
            a[i] = (short)(a[i] + 2);
            a[i - 1]--;
        }
    }
}

ull bin_to_dec(const short *bin, us n) {
    // This function converts a binary number (represented by the array
    // of binary digits) to the decimal equivalent
    ull res = 0;
    us i = 0;

    while (!bin[i]) i++;

    for (; i < n; i++) {
        res = (res << 1) + (ull)bin[i];
    }

    return res;
}


ul count_results(short *a, short *b, us n, Node *polynomial) {
    // This function counts the result (a number of possible inputs within
    // the specified range which will result in the 1 state in the output)

    // Calculate a number of iterations (different inputs that we have
    // to check)
    subtract_bin(b, a, n);
    ull iter_count = bin_to_dec(b, n);

    // Prepare variables
    bool *is_coefficient = (bool*)calloc(n, sizeof(bool));
    Node *curr = polynomial->next;
    ul total = 0;
    us ones_count = 0;

    // Count a number of ones in the inout which are placed on positions
    // corresponding to the polynomial coefficients. If this number is odd,
    // we will get 1 state in the output. Otherwise, we will get 0 state.
    while (curr) {
        ones_count = (us)(ones_count + a[curr->val - 1]);
        is_coefficient[curr->val - 1] = true;
        curr = curr->next;
    }
    total += ones_count % 2;

    // In a loop, update the current input (increment a binary number) and
    // calculate a number of ones for the new input (in an effective way,
    // by checking only fields which values were modified)
    for (ull i = 0; i < iter_count; i++) {
        ones_count = count_ones_next_bin(a, n, ones_count, is_coefficient);
        total += ones_count % 2;
    }

    // Release memory
    free(is_coefficient);

    return total;
}

us count_ones_next_bin(short *bin, us n, us prev_count, const bool *is_coefficient) {
    us i = (us)(n - 1);

    // If the last (youngest) bit of a binary number is 0, we don't have to
    // shift bits, so we will simply replace 0 by 1 (and update the current
    // number of ones)
    if (!bin[i]) {
        bin[i] = 1;
        if (is_coefficient[i]) prev_count++;
    // Otherwise, we have to carry a transfer bit to the left until we reach
    // a 0 bit.
    } else {
        while (true) {
            if (!bin[i]) {
                bin[i] = 1;
                // Update the current number of ones in the output
                if (is_coefficient[i]) prev_count++;
                break;
            }
            // Replace all bits on the way with zeros
            bin[i] = 0;
            // Update the current number of ones in the output
            if (is_coefficient[i]) prev_count--;
            i--;
        }
    }
    // Return updated number of ones
    return prev_count;
}
