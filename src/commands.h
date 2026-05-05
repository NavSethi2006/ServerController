#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "utils/configs.h"

typedef void (*command_func)(int client);

typedef struct {
    const char *name;
    command_func func;
}Command;

void cmd_start(int client);
void cmd_stop(int client);
void handle_commands(char *cmd, int client);

Command commands[] = {
    {"START", cmd_start},
    {"STOP", cmd_stop}
};
