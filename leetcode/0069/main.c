#include <stdio.h>
#include <stdint.h>

int mySqrt(int x) {
    int64_t t = x;
    int64_t ln = 0, rn = x;
    while (ln <= rn) {
        int64_t mid = ln + (rn - ln) / 2;
        if (mid * mid < x) {
            if ((mid + 1) * (mid + 1) > x) {
                return mid;
            }
            ln = mid + 1;
        } else if (mid * mid > x) {
            rn = mid - 1;
        } else {
            return mid;
        }
    }
    return -1;
}

int main() {
    printf("%d\n", mySqrt(9));
    return 0;
}
