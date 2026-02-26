#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

char * skip_space(char *s) {
    while (isspace(*s)) s++;
    return s;
}

int symbol(char **ps) {
    char *s = *ps;
    if (*s == '+') {
        s++;
        *ps = s;
        return 1;
    }
    if (*s == '-') {
        s++;
        *ps = s;
        return -1;
    }
    return 1;
}

int64_t read_num(char *s) {
    int64_t r = 0;
    int i = 0;
    while (1) {
        if (s[i] < '0' || s[i] > '9') {
            return r;
        }
        r = r * 10 + s[i] - '0';
        if (r > ((int64_t)1 << 32)) {
            return r;
        }
        i++;
    }
    return r;
}

int myAtoi(char* s) {
    s = skip_space(s);
    int sym = symbol(&s);
    int64_t r = read_num(s);
    if (sym > 0) {
        if (r > 2147483647) r = 2147483647;
    } else {
        if (r > 2147483648) r = 2147483648;
    }
    return sym > 0 ? r : -r;
}

int main() {
    printf("%d\n", myAtoi("123"));
    printf("%d\n", myAtoi("  +123"));
    printf("%d\n", myAtoi("0000000000012345678"));
    printf("%d\n", myAtoi("-123111111111111111"));
}