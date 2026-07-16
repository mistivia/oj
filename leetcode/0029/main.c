#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t value;    /* divisor doubled some number of times */
    uint32_t multiple; /* how many times the divisor that value is */
} Step;

int divide(int dividend, int divisor) {
    bool neg = (dividend < 0) != (divisor < 0);

    uint32_t rem = dividend < 0 ? -(uint32_t)dividend : (uint32_t)dividend;
    uint32_t div = divisor < 0 ? -(uint32_t)divisor : (uint32_t)divisor;

    /* Doubling 1 up to 2^31 is the longest possible run, so 32 always fits. */
    Step steps[32];
    int  len = 0;

    /* Double the divisor until doing so would overflow or pass the dividend.
     * `value <= rem - value` covers both: value never exceeds rem here. */
    if (div <= rem) {
        Step step  = {.value = div, .multiple = 1};
        steps[len++] = step;
        while (step.value <= rem - step.value) {
            step.value += step.value;
            step.multiple += step.multiple;
            steps[len++] = step;
        }
    }

    uint32_t quotient = 0;
    for (int i = len - 1; i >= 0; i--) {
        if (rem >= steps[i].value) {
            rem -= steps[i].value;
            quotient += steps[i].multiple;
        }
    }

    if (neg) {
        if (quotient >= 2147483648u) {
            return INT32_MIN;
        }
        return -(int32_t)quotient;
    }
    if (quotient > 2147483647u) {
        return INT32_MAX;
    }
    return (int32_t)quotient;
}
