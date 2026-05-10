/**
 * modify to your program configs
 * I'd suggest you gather all your configs from the file you intially created
 * and keep em in here. would make it easier to populate whatever your making
 */


#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "logger.h"
#include "file_handle.h"
#include "arpa/inet.h"

typedef struct {

    uint32_t ip;

    uint32_t broadcastip;

    uint32_t targetip;

    uint16_t port;

    uint16_t targetport;

    unsigned char wolmac[6];

    char auth_password[128];

} CONFIG;

void load_ip();
void load_port();
void load_auth_password();
void load_all();

CONFIG *global_config();

#endif


