#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int reverse(int x){
    if (x == INT_MIN) {
        return 0;
    }
    char buf[10];
    memset(buf, 0, 10);
    int i = 0;
    bool pos = x > 0 ? true : false;
    uint32_t ux = x > 0 ? x : -x;
    while (ux > 0) {
        buf[i] = ux % 10;
        ux = ux / 10;
        i++;
    }
    uint32_t r = 0;
    int len = i;
    for (int i = 0; i < len && i < 9; i++) {
        r = r * 10 + buf[i];
    }
    if (len <= 9) {
        return pos ? r : -r;
    }
    if (r < 214748364) {
        r = r * 10 + buf[9];
        return pos ? r : -r;
    }
    if (r > 214748364) {
        return 0;
    }
    if (pos) {
        if (buf[9] > 7) {
            return 0;
        }
    } else {
        if (buf[9] > 8) {
            return 0;
        }
    }
    r = r * 10 + buf[9];
    return pos ? r : -r;
}

int main() {
    printf("%d\n", reverse(123));
    printf("%d\n", reverse(-123));
}