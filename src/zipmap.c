#include "zipmap.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "endian.h"

#define ZM_END 0xff
#define ZM_BIGLEN 0xfe
#define ZM_IS_END(entry) ((uint8_t)entry[0] == ZM_END)

uint32_t zipmap_entry_len_size(const char *entry) {
    uint32_t size = 0;
    if ((uint8_t)entry[0] < ZM_BIGLEN) {
        size = 1;
    } else if ((uint8_t)entry[0] == ZM_BIGLEN) {
        size = 5;
    }

    return size;
}

uint32_t zipmap_entry_strlen(const char *entry) {
    uint32_t entry_len_size;
    uint32_t str_len;

    entry_len_size = zipmap_entry_len_size(entry);
    if (entry_len_size == 1) {
        return (uint8_t)entry[0];
    } else if (entry_len_size == 5) {
        memcpy(&str_len, entry + 1, 4);
        memrev32ifbe(&str_len);
        return str_len;
    } else {
        return 0;
    }
}

uint32_t zipmap_entry_len (const char *entry) {
    return zipmap_entry_len_size(entry) + zipmap_entry_strlen(entry);
}

void push_zipmap(lua_State *L, const char *zm) {
    int klen, vlen;
    char *key, *val;

    ++zm;
    while (!ZM_IS_END(zm)) {
        /* key */
        klen = zipmap_entry_strlen(zm);
        key = malloc(klen + 1);
        if (!key) {
            logger(ERROR, "Exited, as malloc at push_zipmap(key).\n");
        }
        memcpy(key, zm + zipmap_entry_len_size(zm), klen);
        key[klen] = '\0';
        zm += zipmap_entry_len(zm);

        /* value */
        vlen = zipmap_entry_strlen(zm);
        val = malloc(vlen + 1);
        if (!val) {
            logger(ERROR, "Exited, as malloc at push_zipmap(value).\n");
        }
        memcpy(val, zm + zipmap_entry_len_size(zm) + 1, vlen);
        val[vlen] = '\0';
        zm += (zipmap_entry_len(zm) + 1);

        script_pushtablestring(L, key, val);
        free(key);
        free(val);
    }
}

/* confusing what if size == 5? */
void zipmap_dump(const char *entry) {
    int i = 0, len , klen, vlen;
    char *key, *val;

    len = (uint8_t)zm[0];
    ++zm;

    while (!ZM_IS_END(zm)) {
        /* key */
        klen = zipmap_entry_strlen(zm);
        key = malloc(klen + 1);
        if (!key) {
            logger(ERROR, "Exited, as malloc at zipmap_dump(key).\n");
        }
        memcpy(key, zm + zipmap_entry_len_size(zm), klen);
        key[klen] = '\0';
        zm += zipmap_entry_len(zm);

        /* value */
        vlen = zipmap_entry_strlen(zm);
        val = malloc(vlen + 1);
        if (!val) {
            logger(ERROR, "Exited, as malloc at zipmap_dump(value).\n");
        }
        memcpy(val, zm + zipmap_entry_len_size(zm) + 1, vlen);
        val[vlen] = '\0';
        zm += (zipmap_entry_len(zm) + 1);

        printf("key is %s, value is %s\n", key, val);
        free(key);
        free(val);
        i += 2;
    }

    if (len >= 254 && i == len) {
        logger(ERROR, "Exited, as length error at zipmap_dump.\n");
        exit(1);
    }
}
