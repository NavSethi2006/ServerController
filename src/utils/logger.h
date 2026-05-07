#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

typedef enum {
    SUCCESS,
    FAILED,
    INFO
} STATUS;

void create_log();
void LOG(char* to_log, STATUS status);

#endif
