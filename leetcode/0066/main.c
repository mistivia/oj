#include <stdlib.h>

// begin

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* plusOne(int* digits, int digitsSize, int* returnSize) {
    int *buffer = malloc((digitsSize + 1) * sizeof(int));
    for (int i = 0; i < digitsSize; i++) {
        buffer[i] = digits[digitsSize - 1 - i];
    }
    buffer[0]++;
    int carry = 0;
    for (int i = 0; i < digitsSize; i++) {
        int val = buffer[i] + carry;
        carry = val / 10;
        buffer[i] = val % 10;
    }
    int len = digitsSize;
    if (carry) {
        len++;
        buffer[digitsSize] = 1;
    }
    int lp = 0, rp = len - 1;
    while (lp <= rp) {
        int t = buffer[lp];
        buffer[lp] = buffer[rp];
        buffer[rp] = t;
        lp++;
        rp--;
    }
    *returnSize = len;
    return buffer;
}

// end

