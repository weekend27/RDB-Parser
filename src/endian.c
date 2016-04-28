// #define TEST_MAIN 1

#include <string.h>

typedef int bool;
enum {false, true};


/* judge the machine storage method: little endian or big endian */
bool isBigEndian() {
    union NUM {
        int a;
        char b;
    }num;
    num.a = 0x1234;
    if (num.b == 0x12) {
        return true;
    }
    return false;
}


/* Toggle the 16 bit unsigned integer pointed by *p from little endian to big endian */
void memrev16(void *p) {
    unsigned char *x = p;
    const char *t = p;

    int len = strlen(t);
    int i;
    for (i = 0; i < len - 1; i += 2) {
        x[i] = x[i] ^ x[i+1];
        x[i+1] = x[i] ^ x[i+1];
        x[i] = x[i] ^ x[i+1];
    }
}


/* Toggle the 32 bit unsigned integer pointed by *p from little endian to big endian */
void memrev32(void *p) {
    unsigned char *x = p;
    const char *t = p;

    int len = strlen(t);

    int i;
    for (i = 0; i < len - 3; i += 4) {
        x[i] = x[i] ^ x[i+3];
        x[i+3] = x[i] ^ x[i+3];
        x[i] = x[i] ^ x[i+3];

        x[i+1] = x[i+1] ^ x[i+2];
        x[i+2] = x[i+1] ^ x[i+2];
        x[i+1] = x[i+1] ^ x[i+2];
    }
}


/* Toggle the 64 bit unsigned integer pointed by *p from little endian to big endian */
void memrev64(void *p) {
    unsigned char *x = p;
    const char *t = p;

    int len = strlen(t);
    int i;
    for (i = 0; i < len - 7; i += 8) {
        x[i] = x[i] ^ x[i+7];
        x[i+7] = x[i] ^ x[i+7];
        x[i] = x[i] ^ x[i+7];

        x[i+1] = x[i+1] ^ x[i+6];
        x[i+6] = x[i+1] ^ x[i+6];
        x[i+1] = x[i+1] ^ x[i+6];

        x[i+2] = x[i+2] ^ x[i+5];
        x[i+5] = x[i+2] ^ x[i+5];
        x[i+2] = x[i+2] ^ x[i+5];

        x[i+3] = x[i+3] ^ x[i+4];
        x[i+4] = x[i+3] ^ x[i+4];
        x[i+3] = x[i+3] ^ x[i+4];
    }
}


#ifdef TEST_MAIN
#include <stdio.h>

int main() {
    if (isBigEndian()) {
        printf("BigEndian\n");
    } else {
        printf("LittleEndian\n");
    }

    char buf[16];
    char *s = "abcdefghijklmnop";
    int len = strlen(s);
    printf("len = %d\n", len);

    sprintf(buf, "abcdefghijklmnop");
    memrev16(buf);
    printf("%s\n", buf);

    sprintf(buf, "abcdefghijklmnop");
    memrev32(buf);
    printf("%s\n", buf);

    sprintf(buf, "abcdefghijklmnop");
    memrev64(buf);
    printf("%s\n", buf);

    return 0;
}

#endif
