#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"

FILE *setup_config();

void write_file(FILE *fp, const char *string);

void ban_ip(char* ip);

void save_file_configs(const char* label, const char *src) ;

char* read_file_configs(const char* label);

bool check_for_banned_ip(char* ip);

#endif