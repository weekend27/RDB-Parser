/*
 * rdb parser for redis.
 * author: @weekend27
 * source code: https://github.com/weekend27/RDB-Parser
 * reference: https://github.com/git-hulk/rdbtools
 * reference: https://github.com/antirez/redis
 * date: 2016-03-27
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "rdb.h"
#include "log.h"

static void usage() {
    fprintf(stderr, "USAGE: ./rdbtools [-f file] -V -h\n");
    fprintf(stderr, "\t-V --version \n");
    fprintf(stderr, "\t-h --help show usage \n");
    fprintf(stderr, "\t-f --file specify which rdb file would be parsed.\n");
    fprintf(stderr, "\t-s --file specify which lua script, default is ../scripts/example.lua\n");
    fprintf(stderr, "\t Notice: This tool only test on redis 2.2 and 2.4, 2.6, 2.8.\n\n");
}

int main(int argc, char **argv) {
    int c;
    char *rdb_file = NULL;
    char *lua_file = NULL;
    int is_show_help = 0, is_show_version = 0;
    char short_options[] = {"hVf:s:"};
    lua_State *L;

    
}
