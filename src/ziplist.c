#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ziplist.h"
#include "util.h"
#include "log.h"
#include "endian.h"

uint32_t ziplist_prev_len_size(const char *s) {
    return ((uint8_t)s[0] < ZIPLIST_BIGLEN) ? 1 : 5;
}

uint8_t ziplist_entry_is_str(const char *entry) {
    uint8_t enc;
    enc = (uint8_t)entry[ziplist_prev_len_size(entry)];
    enc &= ZIP_ENC_STR_MASK;
    if (enc == ZIP_ENC_STR_06B || enc == ZIP_ENC_STR_14B || ZIP_ENC_STR_32B) {
        return 1;
    } else {
        return 0;
    }
}

uint32_t ziplist_entry_strlen(const char *entry) {
    uint8_t enc;
    uint32_t pos;

    pos = ziplist_prev_len_size(entry);
    enc = (uint8_t)entry[pos];
    enc &= ZIP_ENC_STR_MASK;
    if (enc == ZIP_ENC_STR_06B) {
        return (uint32_t)(entry[pos] & (~ZIP_ENC_STR_MASK));
    } else if (enc == ZIP_ENC_STR_14B) {
        return (uint32_t)((((uint8_t)entry[pos] & (~ZIP_ENC_STR_MASK)) << 8) | (uint8_t)entry[pos + 1]);
    } else if (enc == ZIP_ENC_STR_32B) {
        return ((uint8_t)entry[pos + 1] << 24) | ((uint8_t)entry[pos + 2] << 16) | ((uint8_t)entry[pos + 3] << 8) | ((uint8_t)entry[pos + 4]);
    }

    return 0;
}

uint32_t ziplist_entry_size(const char *entry) {
    uint8_t enc;
    uint32_t size, pos;

    /* entry->encoding->length >= 1 Byte */
    size = 1;

    pos = ziplist_prev_len_size(entry);
    enc = (uint8_t)entry[pos];
    size += ziplist_prev_len_size(entry);

    if (enc == ZIP_ENC_INT08B) {
        size += 1;
    } else if (enc == ZIP_ENC_INT16B) {
        size += 2;
    } else if (enc == ZIP_ENC_INT24B) {
        size += 3;
    } else if (enc == ZIP_ENC_INT32B) {
        size += 4;
    } else if (enc == ZIP_ENC_INT64B) {
        size += 8;
    } else if ((enc & ZIP_ENC_STR_MASK) == ZIP_ENC_STR_06B) {
        size += ziplist_entry_strlen(entry);
    } else if ((enc & ZIP_ENC_STR_MASK) == ZIP_ENC_STR_14B) {
        size += 1;
        size += ziplist_entry_strlen(entry);
    } else if ((enc & ZIP_ENC_STR_MASK) == ZIP_ENC_STR_32B) {
        size += 4;
        size += ziplist_entry_strlen(entry);
    }

    return size;
}

char* ziplist_entry_str(const char *entry) {
    uint8_t enc;
    uint32_t pre_len_size, len_size = 1, str_len;
    char *content, *str = NULL;

    pre_len_size = ziplist_prev_len_size(entry);
    enc = entry[pre_len_size] & ZIP_ENC_STR_MASK;
    if (enc == ZIP_ENC_STR_14B) {
        len_size += 1;
    } else if (enc == ZIP_ENC_STR_32B) {
        len_size += 4;
    }

    content = (char *)entry + pre_len_size + len_size;
    if (enc == ZIP_ENC_STR_06B || enc == ZIP_ENC_STR_14B || enc == ZIP_ENC_STR_32B) {
        str_len = ziplist_entry_strlen(entry);
        str = malloc(str_len + 1);
        if (!str) {
            logger(ERROR, "Exited, as malloc failed at ziplist entry string.\n");
        }
        memcpy(str, content, str_len);
        str[str_len] = '\0';
    }

    return str;
}

uint8_t ziplist_entry_int(const char *entry, int64_t *v) {
    int8_t v8;
    int16_t v16;
    int32_t v32;
    int64_t v64;
    uint8_t enc;
    uint32_t pre_len_size;
    char *content;

    pre_len_size = ziplist_prev_len_size(entry);
    content = (char *)entry + pre_len_size + 1;
    enc = entry[pre_len_size];

    if (enc == ZIP_ENC_INT08B) {
        memcpy(&v8, content, sizeof(int8_t))；
        *v = v8;
    } else if (enc == ZIP_ENC_INT16B) {
        memcpy(&v16, content, sizeof(int16_t));
        memrev16ifbe(&v16);
        *v = v16;
    } else if (enc == ZIP_ENC_INT24B) {
        memcpy(&v32, content, 3);
        memrev32ifbe(&v32);
        *v = v32;
    } else if (enc == ZIP_ENC_INT32B) {
        memcpy(&v32, content, sizeof(int32_t));
        memrev32ifbe(&v32);
        *v = v32;
    } else if (enc == ZIP_ENC_INT64B) {
        memcpy(&v64, content, sizeof(int64_t));
        memrev64ifbe(&v64);
        *v = v64;
    } else if ((enc & 0xf0) == 0xf0) {
        v8 = enc & 0x0f;
        *v = v8;
    } else {
        return 0;
    }

    return 1;
}

void push_ziplist_list_or_set (lua_State *L, const char *zl) {
    int64_t i = 0, v;
    char *entry, *str;

    entry = (char *)ZL_ENTRY(zl);
    while (!ZIP_IS_END(entry)) {
        if (ziplist_entry_is_str(entry)) {
            str = ziplist_entry_str(entry);
        } else {
            if (ziplist_entry_int(entry, &v) > 0) {
                str = ll2string(v);
            }
        }
        script_push_list_elem(L, str, i++);
        entry += ziplist_entry_size(entry);
        free(str);
    }
}

void push_ziplist_hash_or_zset(lua_State *L, const char *zl) {
    int64_t v;
    char *entry, *str, *key, *val;

    entry = (char *)ZL_ENTRY(zl);
    while (!ZIP_IS_END(entry)) {
        int i;
        for (i = 0; i < 2; ++i) {
            if (ziplist_entry_is_str(entry)) {
                str = ziplist_entry_str(entry);
            } else {
                if (ziplist_entry_int(entry, &v) > 0) {
                    str = ll2string(v);
                }
            }
            if (i = 0) {
                key = str;
            } else {
                val = str;
            }
            entry += ziplist_entry_size(entry);
        }

        script_pushtablestring(L, key, val);
        free(key);
        free(val);
    }
}

void ziplist_dump(const char *zl) {
    uint32_t i = 0, len;
    char *entry, *str;

    printf("ziplist { \n");
    printf("bytes: %u\n", ZL_BYTES(zl));
    printf("len: %u\n", ZIL_LEN(zl));
    len = ZL_LEN(zl);
    entry = (char *)ZL_ENTRY(zl);
    while (!ZIP_IS_END(entry)) {
        if (ziplist_entry_is_str(entry)) {
            str = ziplist_entry_str(entry);
            if (str) {
                printf("str value: %s\n", str);
                free(str);
            }
        } else {
            int64_t v;
            if (ziplist_entry_int(entry, &v) > 0) {
                printf("int value: %lld\n", v);
            }
        }
        entry += ziplist_entry_size(entry);
        ++i;
    }
    printf("}\n");
    if (i < (0xffff - 1) && i != len) {
        printf("======== Ziplist length ERROR. ========\n");
        exit(1);
    }
}
