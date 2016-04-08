#ifndef _ZIPLIST_H_
#define _ZIPLIST_H_
#include <stdint.h>
#include "script.h"

/* refer to https://github.com/antirez/redis/blob/unstable/src/ziplist.c */

#define ZIPLIST_BIGLEN 254
#define ZIPLIST_END 0xff

#define ZIP_ENC_INT08B 0xfe
#define ZIP_ENC_INT16B (0xc0 | 0 << 4)
#define ZIP_ENC_INT32B (0xc0 | 1 << 4)
#define ZIP_ENC_INT64B (0xc0 | 2 << 4)
#define ZIP_ENC_INT24B (0xc0 | 3 << 4)

#define ZIP_ENC_STR_06B (0 << 6)
#define ZIP_ENC_STR_14B (1 << 6)
#define ZIP_ENC_STR_32B (2 << 6)

#define ZIP_ENC_INT8  0xfe
#define ZIP_ENC_INT16 0xc0
#define ZIP_ENC_INT24 0xf0
#define ZIP_ENC_INT32 0xd0
#define ZIP_ENC_INT64 0xe0

#define ZIP_ENC_STR_6B  (0 << 6)
#define ZIP_ENC_STR_14B (1 << 6)
#define ZIP_ENC_STR_32B (2 << 6)

#define ZIP_ENC_STR_MASK 0xc0
#define ZIP_IS_END(entry) ((uint8_t)entry[0] == ZIPLIST_END)
#define ZL_BYTES(zl) *((uint32_t*)(zl))
#define ZL_LEN(zl) *((uint16_t*)((zl) + 2 * sizeof(uint32_t)))
#define ZL_HDR_SIZE (2 * sizeof(uint32_t) + sizeof(uint16_t))
#define ZL_ENTRY(zl) ((uint8_t*)zl + ZL_HDR_SIZE)

typedef struct {
    uint32_t bytes;
    uint32_t len;
    uint32_t tail;
    char entries[0];
} ziplist;

void push_ziplist_hash_or_zset(lua_State *L, const char *zl);
void push_ziplist_list_or_set (lua_State *L, const char *zl);
void ziplist_dump(const char *s);

#endif
