#ifndef SECURITY_H
#define SECURITY_H

#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>

#include "configs.h"

void sha256(const char *input, char output[65]);
void generate_challange(char* buffer, size_t size);
int auth_handler(int client_socket);

#endif