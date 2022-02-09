#include <stdio.h>


unsigned short find_partition_idx(int arr[], unsigned short length);


int main() {
    // Get input data
    unsigned short n;
    scanf("%hd", &n);
    int arr[n];
    for (unsigned short i = 0; i < n; ++i) {
        scanf("%d", &arr[i]);
    }
    // Search for the partition index and print it out
    printf("%hd", find_partition_idx(arr, n));

    return 0;
}


unsigned short find_partition_idx(int arr[], unsigned short length) {
    // If an array has only one element the only one possible solutions is to partition
    // this array on this element's index
    if (length == 1) return 0;

    unsigned long sum_left, sum_right;
    sum_left = sum_right = 0;
    // Get sum of all values (except the first one) in the array and store it as a sum_right
    for (unsigned short i = 1; i < length; ++i) sum_right += arr[i];
    // If sums are already equal (oly if the right part's sum is equal to 0 and there
    // is no left part as we partition an array on the 0 index), return 0
    if (!sum_right) return 0;
    // Loop till a left_sum is different than a right_sum and increase a left_sum and
    // decrease a right_sum
    for (unsigned short i = 1; i < length; ++i) {
        sum_left += arr[i - 1];
        sum_right -= arr[i];
        if (sum_left == sum_right) return i;
    }
    // In other cases (if we cannot split an array to two arrays of an equal sum) return -1
    return -1;
}
