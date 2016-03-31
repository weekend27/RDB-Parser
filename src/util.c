#define BOOL int
#define TRUE 1
#define FALSE 0

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char* ll2string(long long v) {
    BOOL flag = FALSE;
    if (v < 0) {
        flag = TRUE;
    }

    long long tmp = abs(v);
    //printf("tmp = %lld\n", tmp);
    int len = 2;    /* include '/0' in the end of the string */
    while ((tmp /= 10) > 0) {
        len++;
    }
    if (flag) {
        len++;
    }
    //printf("len = %d\n", len);

    char *buf;
    buf = malloc(len);
    if (!buf) {
        fprintf(stderr, "Exited, as malloc failed at ll2string.\n");
        exit(1);
    }

    int i = len - 2;
    tmp = abs(v);
    if (tmp == 0) {
        buf[0] = '0';
    }
    while (tmp > 0) {
        buf[i--] = tmp % 10 + '0';
        tmp /= 10;
    }
    if (flag) {
        buf[0] = '-';
    }
    buf[len - 1] = '\0';

    return buf;
}


#ifdef TEST_MAIN

int main() {
    int v1 = 0;
    int v2 = 24;
    int v3 = 897;
    int v4 = 8776564;
    int v5 = -999;
    int v6 = -0;
    printf("%d ---> \"%s\"\n", v1, ll2string(v1));
    printf("%d ---> \"%s\"\n", v2, ll2string(v2));
    printf("%d ---> \"%s\"\n", v3, ll2string(v3));
    printf("%d ---> \"%s\"\n", v4, ll2string(v4));
    printf("%d ---> \"%s\"\n", v5, ll2string(v5));
    printf("%d ---> \"%s\"\n", v6, ll2string(v6));
    return 0;
}

#endif
