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
    fprintf(stderr, "USAGE: ./RDB-Parser [-f file] -v -h\n");
    fprintf(stderr, "\t-v --version \n");
    fprintf(stderr, "\t-h --help show usage \n");
    fprintf(stderr, "\t-f --file specify which rdb file would be parsed.\n");
    fprintf(stderr, "\t-s --file specify which lua script, default is ../scripts/example.lua\n");
    fprintf(stderr, "\t Notice: This tool only test on redis 2.2, 2.4, 2.6, 2.8, 3.1.\n");
}

int main(int argc, char **argv) {
    int c;
    char *rdb_file = NULL;
    char *lua_file = NULL;
    int is_show_help = 0, is_show_version = 0;
    char short_options[] = {"hvf:s:"};
    lua_State *L;

    struct option long_options[] = {
        { "help", no_argument, NULL, 'h' }, /* help */
        { "version", no_argument, NULL, 'v' }, /* version */
        { "rdb-file-path", required_argument, NULL, 'f' }, /* rdb file path */
        { "lua-file-path", required_argument, NULL, 's' }, /* script file path */
        { NULL, 0, NULL, 0 }
    };

    for (;;) {
        c = getopt_long(argc, argv, short_options, long_options, NULL);
        if (c == -1) {
            break;
        }
        switch(c) {
            case 'h':
                is_show_help = 1;
                break;
            case 'v':
                is_show_version = 1;
                break;
            case 'f':
                rdb_file = optarg;
                break;
            case 's':
                lua_file = optarg;
                break;
            default:
                exit(0);
        }
    }

    if (is_show_version) {
        fprintf(stderr, "\nRDB PARSER VERSION: 0.1\n\n");
    }
    if (is_show_help) {
        usage();
    }
    if (is_show_help || is_show_version) {
        exit(0);
    }
    if (!rdb_file) {
        logger(ERROR, "You must specify rdb file by option -f filepath.\n");
    }
    if (!lua_file) {
        lua_file = "../scripts/example.lua";
    }
    if (access(rdb_file, R_OK) != 0) {
        logger(ERROR, "rdb file %s can not be read!\n", rdb_file);
    }
    if (access(lua_file, R_OK) != 0) {
        logger(ERROR, "lua file %s can not be read!\n", lua_file);
    }

    L = script_init(lua_file);
    rdb_load(L, rdb_file);
    lua_close(L);
    return 0;
}
