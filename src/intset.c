#include "intset.h"
#include <stdio.h>
#include <string.h>

#include "endian.h"

uint8_t intset_get(intset *is, int pos, int64_t *v) {
    int64_t v64;
    int32_t v32;
    int16_t v16;

    if (pos >= is->length) return 0;

    if (is->encoding == sizeof(int64_t)) {
        memcpy(&v64, (int64_t*)is->contents + pos, sizeof(int64_t));
        memrev64ifbe(&v64);
        *v = v64;
    } else if (is->encoding == sizeof(int32_t)) {
        memcpy(&v32, (int32_t*)is->contents + pos, sizeof(int32_t));
        memrev32ifbe(&v32);
        *v = v32;
    } else {
        memcpy(&v16, (int16_t*)is->contents + pos, sizeof(int16_t));
        memrev16ifbe(&v16);
        *v = v16;
    }

    return 1;
}

void intset_dump(intset *is) {
    printf("encoding: %d\n", (int)is->encoding);
    printf("length: %d\n", (int)is->length);

    int i;
    int64_t v;
    printf("element {\t");
    for (i = 0; i < is->length; ++i) {
        intset_get(is, i, &v);
        printf("%lld\t", (long long int)v);
    }
    printf("}\n");
}
