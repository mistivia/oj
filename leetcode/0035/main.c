#include <stdio.h>

// begin

int searchInsert(int* nums, int numsSize, int target) {
    int lp = 0, rp = numsSize - 1;
    int mid = -1;
    while (lp <= rp) {
        mid = lp + (rp - lp) / 2;
        if (target > nums[mid]) {
            lp = mid + 1;
        } else if (target < nums[mid]) {
            rp = mid - 1;
        } else {
            return mid;
        }
    }
    if (target > nums[mid]) mid++;
    return mid;    
}

// end

int main() {
    int arr[] = {1,2,4,5,6};
    printf("%d\n", searchInsert(arr, 5, 3));
}