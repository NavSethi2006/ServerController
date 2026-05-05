#include "configs.h"

CONFIG *config;

void load_ip() {
    char *read = read_file_configs("ip");
    if (!read) {
        LOG("Failed to read IP from config", FAILED);
        config->ip = 0;
        return;
    }
    LOG("Retrieved ip from config", INFO);
    struct in_addr addr;
    if (inet_pton(AF_INET, read, &addr) != 1) {
        LOG("Invalid IP format", FAILED);
        config->ip = 0;
        free(read);
        return;
    }
    config->ip = addr.s_addr;
    LOG("IP conversion successful", SUCCESS);
    free(read);
}

void load_port() {
    char *read = read_file_configs("port");
    LOG("Retreived port from config", INFO);
    char *endptr;
    unsigned long val = strtoul(read, &endptr, 10);

    if(endptr == read || *endptr != '\0' || val > UINT16_MAX) {
        LOG("Conversion from string to int has failed, cant get port number", FAILED);
        config->port = 0;
    } else {
        uint16_t result = (uint16_t)val;
        LOG("Conversion from string to int", SUCCESS);
        config->port = result;
    }

    free(read);
}

void load_auth_password() {
    char *read = read_file_configs("password");
    LOG("Retreived password from config", INFO);
    config->auth_password = read;
    free(read);

}

void load_wolmac() {
    char *read = read_file_configs("targetmac");
    LOG("Retreived mac for WOL server from config", INFO);
    config->wolmac = read;
    free(read);
}

void load_targetip() {
    char *read = read_file_configs("mac");
    if (!read) {
        LOG("Failed to read MAC from config", FAILED);
        return;
    }
    uint8_t mac[6];
    int ret_val = sscanf(read, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                  &mac[0], &mac[1], &mac[2],
                  &mac[3], &mac[4], &mac[5]) == 6;
    if (!ret_val) {
        LOG("Invalid MAC format", FAILED);
        free(read);
        return;
    }
    LOG("MAC parsed successfully", SUCCESS);
    free(read);
}

void load_all() {
    config = malloc(sizeof(CONFIG));
    if (!config) {
        LOG("CRITICAL : Not enough memory for CONFIG", FAILED);
        exit(EXIT_FAILURE);
    }

    load_ip();
    load_port();
    load_auth_password();
    load_wolmac();
}



CONFIG *global_config() {
    return config;
}
