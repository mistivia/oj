#include <stdlib.h>
#include <string.h>

typedef struct {
    int len;
    char *buf;
    int capacity;
} String;

void String_push(String *self, char c) {
    if (self->len + 2 >= self->capacity) {
        int new_capacity = self->capacity * 2;
        self->buf = realloc(self->buf, new_capacity);
        self->capacity = new_capacity;
    }
    self->buf[self->len] = c;
    self->buf[self->len + 1] = '\0';
    self->len++;
}

void String_init(String *self) {
    self->capacity = 16;
    self->buf = malloc(16);
    self->len = 0;
    self->buf[0] = '\0';
}

char* str_reverse(char *s) {
    int lp = 0, rp = strlen(s) - 1;
    s = strdup(s);
    while (lp <= rp) {
        char tmp = s[lp];
        s[lp] = s[rp];
        s[rp] = tmp;
        rp--;
        lp++;
    }
    return s;
}

char* addBinary(char* a, char* b) {
    a = str_reverse(a);
    b = str_reverse(b);
    String ret;
    String_init(&ret);
    int len_a = strlen(a);
    int len_b = strlen(b);
    int len = len_a > len_b ? len_a : len_b;
    int carry = 0;
    for (int i = 0; i < len; i++) {
        char ca, cb;
        if (i >= len_a) {
            ca = 0;
        } else {
            ca = a[i] - '0';
        }
        if (i >= len_b) {
            cb = 0;
        } else {
            cb = b[i] - '0';
        }
        char val = ca + cb + carry;
        String_push(&ret, val % 2 + '0');
        carry = val / 2;
    }
    if (carry) String_push(&ret, '1');
    free(a);
    free(b);
    char *oldbuf = ret.buf;
    ret.buf = str_reverse(ret.buf);
    free(oldbuf);
    return ret.buf;
}
