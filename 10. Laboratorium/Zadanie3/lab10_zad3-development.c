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
Node* add_node(Node *tail, us idx);
void free_neighbours_lists(Node *G, us n);
void topological_sort(Node *G, us n, us m, us *result);
void dfs(Node *G, us u, us n, bool *visited, us *result, us *idx);
void get_neighbours_counts(Node *G, us g_size, us *result);
us ll_length(Node *head);
us ll_popleft(Node *head);
void transpose_graph(Node *G, us g_size, us n, us *neighbours_counts);
void free_ll(Node *head);
Node* create_polynomial(Node *G, us n, us m, us *neighbours_counts, const us *t_sorted);
Node* get_new_polynomial(Node *p1_ll, Node *p2_ll);
ul count_results(short *a, short *b, us n, Node *polynomial);
void read_bin_num(short *result, us n);
void subtract_bin(short *a, const short *b, us n);
ull bin_to_dec(const short *bin, us n);
us count_ones_next_bin(short *bin, us n, us prev_count, const bool *is_coefficient);


void print_graph(Node *G, us g_size, us n);
void print_ll(Node *head);
void print_arr(us *arr, us size);
void print_bool_arr(bool *arr, us size);
void print_short_arr(const short *a, us n);


int main() {
    // Get input data
    us n, m, out;
    scanf("%hd %hd %hd", &n, &m, &out);
    us g_size = (us)(n + m + 1);
    // Create an array in which we will store a graph (array of linked lists)
    Node *G = (Node*)malloc(sizeof(Node) * g_size);

    printf("Started creating a graph...\n");

    create_graph(G, n, m);

    printf("Graph created successfully\n\n");
    printf("Graph:\n");
    print_graph(G, g_size, n);
    printf("Started topological sorting...\n");


    us *t_sorted = (us*)malloc(sizeof(us) * m);
    topological_sort(G, n, m, t_sorted);


    printf("Topological sorting results:\n");
    print_arr(t_sorted, m);
    printf("Graph after topological sort:\n");
    print_graph(G, g_size, n);
    printf("\nStarted transposing a graph...\n");

    us *neighbours_counts = (us*)calloc(g_size, sizeof(us));
    get_neighbours_counts(G, g_size, neighbours_counts);


    transpose_graph(G, g_size, n, neighbours_counts);

    printf("Transposed graph:\n");
    print_graph(G, g_size, n);

    printf("\nStarted creating the output polynomial...\n");

    Node *out_polynomial = create_polynomial(G, n, m, neighbours_counts, t_sorted);

    printf("\nThe output polynomial:\n");
    print_ll(out_polynomial);


    // Release memory
    free_neighbours_lists(G, g_size);
    free(neighbours_counts);
    free(t_sorted);
    free(G);

    short *a = (short*)malloc(sizeof(short) * (n));
    short *b = (short*)malloc(sizeof(short) * (n));
    read_bin_num(a, n);
    read_bin_num(b, n);

    printf("%lu", count_results(a, b, n, out_polynomial));

    free_ll(out_polynomial);
    free(a);
    free(b);

    return 0;
}

void create_graph(Node *G, us n, us m) {
    short u, v;
    us i, g_size = (us)(n + m + 1);
    Node **tails = malloc(sizeof(Node*) * g_size);
//    printf("Started creating tails array\n");
    for (i = 0; i < g_size; i = (us)(i + 1)) {
        tails[i] = &G[i];
        tails[i]->next = NULL;
    }
    printf("Created tails array...\n");

    for (i = 1; i <= m; i = (us)(i + 1)) {
        printf("New iteration: i=%hd\n", i);
        scanf("%hd %hd", &u, &v);

        printf("Adding vertices: %hd -> %hd, %hd -> %hd\n", u, i, v, i);

        tails[u + n] = add_node(tails[u + n], (us)(i + n));
        tails[v + n] = add_node(tails[v + n], (us)(i + n));

//        printf("\nGraph:\n");
//        print_graph(G, g_size, n);
    }

    // Free an array of tails pointers
    free(tails);
}

Node* add_node(Node *tail, us idx) {
    Node *v = (Node*)malloc(sizeof(Node));
    v->val = idx;
//    printf("Created a new node: v=%hd\n", v->val);
    tail->next = v;
    v->next = NULL;
    return v;
}

void topological_sort(Node *G, us n, us m, us *result) {
    bool *visited = (bool*)calloc((us)(m + 1), sizeof(bool));
    us idx = m;

    for (us i = 1; i <= m; i = (us)(i + 1)) {
        if (!visited[i]) {
//            printf("\nStarting DFS for i=%hd\n", i);
//            printf("Visited: ");
//            print_bool_arr(visited, m);
//            printf("i=%hd neighbours: ", (us)(i + n));
//            print_ll(G + n + i);

            dfs(G, i, n, visited, result, &idx);
        }
    }

    // Release memory
    free(visited);
}

void dfs(Node *G, us u, us n, bool *visited, us *result, us *idx) {
    visited[u] = true;
    Node *curr = (G + n + u)->next;

    while (curr) {
//        printf("Curr u=%hd neighbour: %hd\n", u, curr->val);
        if (!visited[curr->val - n]) {
//            printf("DFS visit: %hd\n", curr->val);
            dfs(G, (us)(curr->val - n), n, visited, result, idx);
        }
        curr = curr->next;
    }

    *idx = (us)(*idx - 1);
    result[*idx] = u;
}

us ll_length(Node *head) {
    Node *curr = head->next;
    us count = 0;

    while (curr) {
        count = (us)(count + 1);
        curr = curr->next;
    }

    return count;
}

void get_neighbours_counts(Node *G, us g_size, us *result) {
    for (us i = 0; i < g_size; i = (us)(i + 1)) {
        result[i] = ll_length(&G[i]);
    }
}

void transpose_graph(Node *G, us g_size, us n, us *neighbours_counts) {
    Node **tails = malloc(sizeof(Node*) * g_size);
    for (us i = 0; i < g_size; i = (us)(i + 1)) {
        tails[i] = &G[i];
        while (tails[i]->next) tails[i] = tails[i]->next;
    }

//    printf("\nNeighbours counts:\n");
//    print_arr(neighbours_counts, g_size);
    bool removed_tail;
    for (us u = 0; u < g_size; u = (us)(u + 1)) {
//        printf("\nCurrently in vertex: %hd (num of neighbours: %hd)\n", u, neighbours_counts[u]);
        removed_tail = false;
        for (us i = 0; i < neighbours_counts[u]; i = (us)(i + 1)) {
            if ((G + u)->next == tails[u]) removed_tail = true;
            us v = ll_popleft(&G[u]);
            if (removed_tail) tails[u] = &G[u];
            printf("Got v=%hd\n", v);
            if (!((G + u)->next)) tails[u]->next = NULL;
            tails[v] = add_node(tails[v], u);

//            printf("(While transposing) graph:\n");
//            print_graph(G, g_size, n);
        }
    }

    // Release memory
    free(tails);
}

us ll_popleft(Node *head) {
    Node *removed = head->next;
    us v = removed->val;
    head->next = removed->next;
    free(removed);
    return v;
}

Node* create_polynomial(Node *G, us n, us m, us *neighbours_counts, const us *t_sorted) {
    us g_size = (us)(n + m + 1);
    Node *polynomials = malloc(sizeof(Node) * g_size);

    for (us i = 0; i < g_size; i = (us)(i + 1)) {
        if (i < n) add_node(&polynomials[i], (us)(n - i));
        else (polynomials + i)->next = NULL;
    }

    printf("Initial polynomials:\n");
    print_graph(polynomials, g_size, n);

    us u, v, w;
    for (us i = 0; i < m; i = (us)(i + 1)) {
        u = (us)(t_sorted[i] + n);
        v = (G + u)->next->val;
        w = (G + u)->next->next->val;

//        printf("Creating a polynomial for u=%hd (parents: v=%hd, w=%hd)\n", u, v, w);

        (polynomials + u)->next = get_new_polynomial(&polynomials[v], &polynomials[w]);
//
//        printf("New polynomial: ");
//        print_ll(&polynomials[u]);
//        printf("Neighbours counts:\n");
        neighbours_counts[v]--;
        neighbours_counts[w]--;

//        print_arr(neighbours_counts, g_size);

        if (!neighbours_counts[v]) {
//            printf("Removing polynomial for %hd (%hd)\n", v, (short)(v - n));
            free_ll((polynomials + v)->next);
            (polynomials + v)->next = NULL;
        }
        if (!neighbours_counts[w]) {
//            printf("Removing polynomial for %hd (%hd)\n", w, (short)(w - n));
            free_ll((polynomials + w)->next);
            (polynomials + w)->next = NULL;
        }
    }

    Node *head = (Node*)malloc(sizeof(Node));
    head->next = (polynomials + n + t_sorted[m - 1])->next;

    printf("\nPolynomials before freeing memory:\n");
    print_graph(polynomials, g_size, n);

    // Release memory
    free(polynomials);

    return head;
}

Node* get_new_polynomial(Node *p1_ll, Node *p2_ll) {
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
    char *chars = (char*)malloc((us)(n + 1) * sizeof(char));
    scanf("%s", chars);

    for (us i = 0; i < n; i = (us)(i + 1)) result[i] = (short)(chars[i] - '0');

    free(chars);
}

void subtract_bin(short *a, const short *b, us n) {
    printf("\nIn subtract:\n");
    printf("a: ");
    print_short_arr(a, n);
    printf("b: ");
    print_short_arr(b, n);

    for (short i = (short)(n - 1); i >= 0; i--) {
        a[i] = (short)(a[i] - b[i]);
        if (a[i] < 0) {
            a[i] = (short)(a[i] + 2);
            a[i - 1]--;
        }
    }
}

ull bin_to_dec(const short *bin, us n) {
    ull res = 0;
    us i = 0;

    while (!bin[i]) i++;

    for (; i < n; i = (us)(i + 1)) {
        res = (res << 1) + (ull)bin[i];
    }

    return res;
}

void free_neighbours_lists(Node *G, us n) {
    for (us i = 0; i < n; i = (us)(i + 1)) {
        free_ll((&G[i])->next);
    }
}

void free_ll(Node *head) {
    Node *temp;

    while (head) {
        temp = head->next;
        free(head);
        head = temp;
    }
}

ul count_results(short *a, short *b, us n, Node *polynomial) {
    subtract_bin(b, a, n);

    printf("\nSubtraction binary result:\n");
    print_short_arr(b, n);
    printf("\n");

    ull iter_count = bin_to_dec(b, n);

    printf("Number of iterations: %I64u\n", iter_count);

    bool *is_coefficient = (bool*)calloc(n, sizeof(bool));
    Node *curr = polynomial->next;
    ul total = 0;
    us ones_count = 0;

    while (curr) {
        ones_count = (us)(ones_count + a[curr->val - 1]);
        is_coefficient[curr->val - 1] = true;
        curr = curr->next;
    }
    total += ones_count % 2;

    printf("Initial number of ones: %hd (total: %ld)\n", ones_count, total);

    for (ull i = 0; i < iter_count; i++) {
        ones_count = count_ones_next_bin(a, n, ones_count, is_coefficient);
        total += ones_count % 2;

//        printf("After i=%I64u iterations: total=%ld (ones count: %hd)\n", i, total, ones_count);
//        printf("Current binary input (a): ");
//        print_short_arr(a, n);
    }

    free(is_coefficient);

    return total;
}

us count_ones_next_bin(short *bin, us n, us prev_count, const bool *is_coefficient) {
    us i = (us)(n - 1);

    if (!bin[i]) {
        bin[i] = 1;
        if (is_coefficient[i]) prev_count++;
    } else {
        while (true) {
            if (!bin[i]) {
                bin[i] = 1;
                if (is_coefficient[i]) prev_count++;
                break;
            }
            bin[i] = 0;
            if (is_coefficient[i]) prev_count--;
            i = (us)(i - 1);
        }
    }

    return prev_count;
}


void print_graph(Node *G, us g_size, us n) {
    for (us i = 0; i < g_size; i = (us)(i + 1)) {
        printf("Idx: %hd (vert: %hd): ", i, (short)(i - n));
        print_ll(&G[i]);
    }
}

void print_ll(Node *head) {
    Node *curr = head->next;

    while (curr) {
        printf("%hd ", curr->val);
        curr = curr->next;
    }
    printf("\n");
}

void print_arr(us *arr, us size) {
    for (us i = 0; i < size; i = (us)(i + 1)) {
        printf("%hd ", arr[i]);
    }
    printf("\n");
}

void print_bool_arr(bool *arr, us size) {
    for (us i = 0; i < size; i = (us)(i + 1)) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void print_short_arr(const short *a, us n) {
    for (us i = 0; i < n; i = (us)(i + 1)) {
        printf("%hd ", a[i]);
    }
    printf("\n");
}
