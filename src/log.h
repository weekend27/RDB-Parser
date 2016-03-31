#ifndef _LOG_H_
#define _LOG_H_

#define C_RED "\033[31m"
#define C_GREEN "\033[32m"
#define C_YELLOW "\033[33m"
#define C_PURPLE "\033[35m"
#define C_NONE "\033[0m"

enum LEVEL {
    DEBUG = 1,
    INFO,
    WARN,
    ERROR
};

void set_log_level(enum LEVEL level);
void set_log_file(char *filename);
void logger(enum LEVEL loglevel, char *format, ...);

#endif
