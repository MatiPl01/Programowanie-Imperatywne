#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAXLENGTH 50

typedef unsigned short us;

void alphabetically_largest_substring(char str[], us length, char result[]);
int compare(const void *a_ptr, const void *b_ptr);


int main() {
    // Prepare variables and read input string
    char str[MAXLENGTH + 1], result[MAXLENGTH + 1];
    us length;
    scanf("%s", str);
    length = (us)strlen(str);

    // Find the alphabetically largest substring
    alphabetically_largest_substring(str, length, result);

    // Print the result
    printf("%s", result);

    return 0;
}


void alphabetically_largest_substring(char str[], us length, char result[]) {
    // Store ASCII code, char index in the source string pairs
    us **helper = malloc(length * sizeof(us*));
    for (us i = 0; i < length; i = (us)(i + 1)) {
        helper[i] = (us*)malloc(2 * sizeof(us));
        helper[i][0] = (us)(str[i] - 'a');
        helper[i][1] = i;
    }

    // Sort this array of pairs by the char code (in a non-increasing order)
    qsort(helper, length, sizeof(helper), compare);

    // Loop over all code-index pairs and rewrite subsequent largest chars
    us code, code_idx, res_idx, new_idx;
    new_idx = res_idx = 0;

    for (us i = 0; i < length; i = (us)(i + 1)) {
        code_idx = helper[i][1];
        // Skip if still cannot rewrite this char (because it was stored before
        // the character previously largest
        if (code_idx < new_idx) continue;
        // Otherwise, we found the next largest character so we can rewrite it
        code = helper[i][0];
        result[res_idx] = (char)('a' + code);
        new_idx = (us)(code_idx + 1);
        res_idx = (us)(res_idx + 1);
    }
    result[res_idx] = '\0';

    // Release memory
    for (us i = 0; i < length; i = (us)(i + 1)) free(helper[i]);
    free(helper);
}


int compare(const void *a_ptr, const void *b_ptr) {
    const us *a = *(const us**) a_ptr;
    const us *b = *(const us**) b_ptr;
    // If have different codes, sort by codes
    if (a[0] < b[0]) return 1;
    // If have the same codes, store first values from a lower index
    if (a[0] == b[0] && a[1] > b[1]) return 1;
    return -1;
}
