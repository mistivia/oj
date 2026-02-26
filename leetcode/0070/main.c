#include <stdlib.h>

int climbStairsImpl(int n, int* cache) {
    if (n == 0) return 1;
    if (n < 0) return 0;
    if (cache[n] > 0) return cache[n];
    int ret = climbStairsImpl(n-1, cache) + climbStairsImpl(n-2, cache);
    cache[n] = ret;
    return ret;
}

int climbStairs(int n) {
    int *cache = malloc(sizeof(int) * (n+1));
    return climbStairsImpl(n, cache);
}
