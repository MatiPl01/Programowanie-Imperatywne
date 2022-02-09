#include <stdio.h>
#include <stdlib.h>


typedef unsigned short us;

void print_seq(const us *arr, us length);
void generate_fibs(unsigned *result, us count);
void generate_sequence(us *result, const unsigned *fibs, us n, unsigned k);


int main() {
    // Prepare variables and get input data
    us n;
    unsigned k;
    scanf("%hd %u", &n, &k);

    // Create an array in which the result will be stored
    us *result = (us*)calloc(n, sizeof(us));
    unsigned *fibs = (unsigned*)malloc(sizeof(unsigned) * (unsigned)(n + 1));

    // Generate Fibonacci numbers used later to generate the resulting
    // sequence
    generate_fibs((unsigned*)fibs, (us)(n + 1));

    // Check if there is k-th sequence of n digits. If k is too large, print -1
    if (k > fibs[n]) {
        printf("-1");
        exit(0);
    }

    // Generate the result
    generate_sequence((us*)result, (unsigned*)fibs, n, k);

    // Print the resulting sequence
    print_seq((us*)result, n);

    // Release memory
    free(result);
    free(fibs);

    return 0;
}


void print_seq(const us *arr, us length) {
    for (us i = 0; i < length; i = (us)(i + 1)) {
        printf("%hd", *(arr + i));
    }
    printf("\n");
}

void generate_fibs(unsigned *result, us count) {
    result[0] = 1;

    if (count > 1) {
        result[1] = 2;

        for (us i = 2; i < count; i = (us)(i + 1))
            result[i] = result[i - 1] + result[i - 2];
    }
}

/*
 This function divides a problem into smaller subproblems by deciding whether
 we should take 1 (and then have to take 0) or take 0. The function makes use of
 Fibonacci numbers which correspond to the number of sequences which begins with
 1 sign on a proper position. e.g. n = 7 and Fibonacci number which corresponds
 to this value is 34 - this it a number in an array of sorted lexicographically
 sequences in which there are no two 1 signs one next to the other. Let's look at
 the example below:

 Let's say we want to know what is the 31th value in a sorted array of 0-1 valid
 7-digit sequences. Notice that 21 < 31 < 34, where 21 is a number of unique 6-digit
 sequences and 34 is a number of unique 7-digit sequences (these are Fibonacci numbers).
 Because 31 > 21, a sequence has to begin with 1 and the next one digit must be 0.
 Now, as we already filled 2 digits, there are 5 digits remaining. Now we have to solve
 the same problem for a number 31 - 21 = 10 considering only 5-digit sequences. Because
 8 < 10 < 13, where 8 is a number of valid 4-digit sequences and 13 is a number of valid
 5-digit sequences, we know that in this 5-digit part our sequence will be greater than
 any 4-digit sequence (as we now look for the 10th 5-digit sequence). Again we have to
 add 10 as the next 2 digits of a sequence and look for the remaining 3 digits and
 10 - 8 = 2nd sequence of these 3-digit sequences. Because 2 <= 3, where 3 is a number of
 valid 2-digit sequences, a 3-digit sequence we are looking for now has to begin with 0
 (because it is one of 2-digit sequences). So we add 0 and start looking for the 2nd
 sequence of 2-digit sequences. As again 2 <= 2, where 2 is a number of 1-digit sequences,
 we have to add another 0 and look for the 2nd sequence of 1-digit sequences. Because
 1 < 2 <= 2, where 1 is a number of 0-digit sequences (there is always only one empty
 sequence), we take 1 as the last one digit of our 7-digit 31th sequence in order.

 When we write result of all the steps listed above and merge them (one after another)
 we will get the resulting sequence (+ sign indicates merging of sequence parts):
 10 + 10 + 0 + 0 + 1  --->  1010001 (31th)
*/

void generate_sequence(us *result, const unsigned *fibs, us n, unsigned k) {
    short i = (short)(n - 1);
    us j = 0;

    while (i >= 0) {
        if (k > *(fibs + i)) {
            if (i > 0) {
                *(result + j) = 1;
                *(result + j + 1) = 0;
                k -= *(fibs + i);
                i = (short)(i - 2);
                j = (us)(j + 2);
            } else {
                *(result + j) = 1;
                break;
            }
        } else {
            *(result + j) = 0;
            i = (short)(i - 1);
            j = (us)(j + 1);
        }
    }
}
