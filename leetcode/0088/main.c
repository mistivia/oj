#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void merge(int* nums1, int nums1Size, int m, int* nums2, int nums2Size, int n) {
    int *buf = malloc(sizeof(int) * (m + n));
    int p1 = 0, p2 = 0;
    int i  = 0;
    while (p1 < m || p2 < n) {
        if (p1 >= m) {
            buf[i] = nums2[p2];
            p2++;
        } else if (p2 >= n) {
            buf[i] = nums1[p1];
            p1++;
        } else {
            if (nums1[p1] < nums2[p2]) {
                buf[i] = nums1[p1];
                p1++;
            } else {
                buf[i] = nums2[p2];
                p2++;
            }
        }
        i++;
    }
    memcpy(nums1, buf, sizeof(int) * (m + n));
}

int main() {
    int arr1[] = {1,2,3,0,0,0};
    int arr2[] = {2,5,6};
    merge(arr1, 3, 3, arr2, 3, 3);
    for (int i = 0; i < 6; i++) {
        printf("%d ", arr1[i]);
    }
    printf("\n");
    return 0;
}
