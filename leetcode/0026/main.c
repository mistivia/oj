#include <stdio.h>

// begin

int removeDuplicates(int* nums, int numsSize) {
    int *p1, p2;
    if (numsSize == 0) return 0;

    int p = 0;
    int cur = nums[0];
    for (int i = 1; i < numsSize; i++) {
        if (nums[i] != cur) {
            p++;
            cur = nums[i];
            nums[p] = nums[i];
        }
    }
    return p+1;
}

// end

int main() {
    int arr[] = {1,2,3,3,4,5};
    printf("%d\n", removeDuplicates(arr, sizeof(arr) / sizeof(int)));
    return 0;
}