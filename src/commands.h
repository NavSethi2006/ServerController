#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "utils/configs.h"

typedef void (*command_func)(int client);

typedef struct {
    const char *name;
    command_func func;
} Command;

void send_wol(int client);
void cmd_stop(int client);

static Command commands[2] = {
    {"START", send_wol},
    {"STOP", cmd_stop}
};

#endif