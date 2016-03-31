#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

static char *log_file = NULL;
static enum LEVEL log_level = INFO;

void set_log_level(enum LEVEL level) {
    log_level = level;
}

void set_log_file(char *filename){
    log_file = filename;
}

void logger(enum LEVEL loglevel, char *format, ...) {
    FILE *fp;
    va_list ap;
    time_t now;
    char buf[4096];
    char t_buf[64];
    char *msg = NULL;
    const char *color = "";

    if (loglevel < log_level) {
        return;
    }

    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    switch (loglevel) {
        case DEBUG: msg = "DEBUG"; color = C_GREEN;  break;
        case INFO:  msg = "INFO";  color = C_YELLOW; break;
        case WARN:  msg = "WARN";  color = C_PURPLE; break;
        case ERROR: msg = "ERROR"; color = C_RED;    break;
    }

    now = time(NULL);
    strftime(t_buf, 64, "%Y-%m-%d %H:%M:%S", localtime(&now));
    fp = (log_file == NULL) ? stdout : fopen(log_file, "a");
    if (log_file) {
        fprintf(fp, "[%s] [%s] %s\n", t_buf, msg, buf);
        fclose(fp);
    } else {
        fprintf(fp, "%s[%s] [%s] %s"C_NONE"\n", color, t_buf, msg, buf);
    }

    if (loglevel >= ERROR) {
        exit(1);
    }
}
