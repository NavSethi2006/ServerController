#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "utils/file_handle.h"
#include "utils/logger.h"
#include "utils/configs.h"
#include "utils/security.h"
#include "utils/file_handle.h"
#include "commands.h"

void setup_server();

void handle_client();

void recieve_packet();

void server_shutdown();

#endif