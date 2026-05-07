#ifndef SECURITY_H
#define SECURITY_H

#include <openssl/sha.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "logger.h"

#include "configs.h"

void sha256(const char *input, char output[65]);
void generate_challange(char* buffer, size_t size);
int auth_handler(int client_socket);

#endif