#ifndef LOGGER_H
#define LOGGER_h

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

typedef enum LOGGING_STATUS {
    SUCCESS,
    FAILED,
    INFO
} STATUS;

void create_log();
void LOG(char* to_log, STATUS status);
void LOG_IP_ADDR(char *infobuff, char* ip_to_log);

#endif
