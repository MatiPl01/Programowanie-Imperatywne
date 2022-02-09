#include <stdio.h>
#include <stdlib.h>


typedef unsigned short us;

void create_connections_matrix(short *matrix, us size, unsigned count);
unsigned count_one_color_triangles(const short *matrix, us size);


int main() {
    // Prepare variables and read input data
    us n;  // Number of points
    unsigned m;  // Number of red segments
    scanf("%hd %u", &n, &m);
    // Create connections matrix to store a color of the segment which connects
    // points 'i' and 'j' together. -1 will represent no connection (only the
    // main diagonal as point 'i' cannot be connected with itself), 0 - connection
    // with a segment of a black color, 1 - connection with a red segment
    // (We create a matrix of size n + 1 in order to add padding on the top and
    // on the left side of a matrix (points are indexed from 1 to n inclusive))
    us size = (us)(n + 1);
    short *T = (short*)calloc((size_t)(size * size), sizeof(short));

    // Read connections of red segments and store them in the connections matrix
    create_connections_matrix((short*)T, size, m);

    // Count all triangles of one color
    printf("%u", count_one_color_triangles((short*)T, size));

    // Release memory
    free(T);

    return 0;
}


void create_connections_matrix(short *matrix, us size, unsigned count) {
    // Fill a diagonal with -1
    for (us i = 1; i < size; i++) {
        *((matrix + i * size) + i) = -1;
    }
    // Fill the remaining part based on input values
    us a, b;
    for (unsigned i = 0; i < count; i++) {
        // Read the vertices which will be connected with a red segment
        scanf("%hd %hd", &a, &b);
        // Apply connections by storing 1 value on matrix[a][b] and matrix[b][a] positions
        *((matrix + a * size) + b) = *((matrix + b * size) + a) = 1;
    }
}

unsigned count_one_color_triangles(const short *matrix, us size) {
    unsigned count = 0;

    // Count all triangles which have all edges of the same color
    short c1, c2, c3;
    for (us i = 1; i < size - 1; i = (us)(i + 1)) {
        for (us j = (us)(i + 1); j < size; j = (us)(j + 1)) {
            for (us k = (us)(i + 1); k < j; k = (us)(k + 1)) {
                c1 = *((matrix + i * size) + j);
                c2 = *((matrix + k * size) + j);
                c3 = *((matrix + k * size) + i);
                if (c1 == c2 && c2 == c3) count++;
            }
        }
    }

    return count;
}
