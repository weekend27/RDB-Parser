#ifndef _ENDIAN_H_
#define _ENDIAN_H_

void memrev16(void *p);
void memrev32(void *p);
void memrev64(void *p);

/* variants of the function doing the actual convertion onlu if the target host is big endian */
/* redis RDB file data is saved in little-endian format*/
/* if BYTE_ORDER is LITTLE_ENDIAN, do nothing:) */
#if (BYTE_ORDER == LITTLE_ENDIAN)
#define memrev16ifbe(p)
#define memrev32ifbe(p)
#define memrev64ifbe(p)
#else
#define memrev16ifbe(p) memrev16(p)
#define memrev32ifbe(p) memrev32(p)
#define memrev64ifbe(p) memrev64(p)
#endif

#endif
