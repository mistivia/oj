#include "stdlib.h"
#include "string.h"
#include "stdio.h"

typedef struct {
    int value;
    char *symbol;
} SymTableEntry;

SymTableEntry sym_table[] = {
    {1000, "M"},
    {900, "CM"},
    {500, "D"},
    {400, "CD"},
    {100, "C"},
    {90, "XC"},
    {50, "L"},
    {40, "XL"},
    {10, "X"},
    {9, "IX"},
    {5, "V"},
    {4, "IV"},
    {1, "I"},
    {-1, NULL},
};

char *concat(char *prev, char *sym) {
    int prevlen = strlen(prev);
    int symlen = strlen(sym);
    char *res = malloc(prevlen + symlen + 1);
    memcpy(res, prev, prevlen);
    memcpy(res + prevlen, sym, symlen);
    res[prevlen + symlen] = '\0';
    free(prev);
    return res;
}

char* intToRoman(int num) {
    char *r = malloc(1);
    r[0] = '\0';
    while (num > 0) {
        for (int i = 0; ; i++) {
            if (sym_table[i].value < 0) break;
            if (sym_table[i].value <= num) {
                num -= sym_table[i].value;
                r = concat(r, sym_table[i].symbol);
                break;
            }
        }
    }
    return r;
}

int main() {
    printf("%s\n", intToRoman(1234));
    return 0;
}