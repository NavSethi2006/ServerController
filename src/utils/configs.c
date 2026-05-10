#include "configs.h"

CONFIG *config;

void load_ip() {

    char *read = read_file_configs("ip");

    if(!read) {

        LOG("Failed to read ip", FAILED);

        exit(EXIT_FAILURE);
    }

    struct in_addr addr;

    if(inet_pton(AF_INET, read, &addr) != 1) {

        LOG("Invalid IP format", FAILED);

        free(read);

        exit(EXIT_FAILURE);
    }

    config->ip = addr.s_addr;

    LOG("Loaded server IP", SUCCESS);

    free(read);
}

void load_broadcastip() {

    char *read = read_file_configs("broadcast");

    if(!read) {

        LOG("Failed to read broadcast ip", FAILED);

        exit(EXIT_FAILURE);
    }

    struct in_addr addr;

    if(inet_pton(AF_INET, read, &addr) != 1) {

        LOG("Invalid broadcast IP", FAILED);

        free(read);

        exit(EXIT_FAILURE);
    }

    config->broadcastip = addr.s_addr;

    LOG("Loaded broadcast IP", SUCCESS);

    free(read);
}

void load_targetip() {

    char *read = read_file_configs("targetip");

    if(!read) {

        LOG("Failed to read target IP", FAILED);

        exit(EXIT_FAILURE);
    }

    struct in_addr addr;

    if(inet_pton(AF_INET, read, &addr) != 1) {

        LOG("Invalid target IP", FAILED);

        free(read);

        exit(EXIT_FAILURE);
    }

    config->targetip = addr.s_addr;

    LOG("Loaded target IP", SUCCESS);

    free(read);
}

void load_port() {

    char *read = read_file_configs("port");

    if(!read) {

        LOG("Failed to read port", FAILED);

        exit(EXIT_FAILURE);
    }

    config->port = (uint16_t)atoi(read);

    LOG("Loaded server port", SUCCESS);

    free(read);
}

void load_targetport() {

    char *read = read_file_configs("targetport");

    if(!read) {

        LOG("Failed to read target port", FAILED);

        exit(EXIT_FAILURE);
    }

    config->targetport = (uint16_t)atoi(read);

    LOG("Loaded target port", SUCCESS);

    free(read);
}

void load_auth_password() {

    char *read = read_file_configs("password");

    if(!read) {

        LOG("Failed to read password", FAILED);

        exit(EXIT_FAILURE);
    }

    strncpy(config->auth_password,
            read,
            sizeof(config->auth_password) - 1);

    config->auth_password[
        sizeof(config->auth_password) - 1
    ] = '\0';

    config->auth_password[
        strcspn(config->auth_password, "\r\n")
    ] = '\0';

    LOG("Loaded auth password", SUCCESS);

    free(read);
}

void load_wolmac() {

    char *read = read_file_configs("targetmac");

    if(!read) {

        LOG("Failed to read MAC", FAILED);

        exit(EXIT_FAILURE);
    }

    int result = sscanf(read,
                        "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",

                        &config->wolmac[0],
                        &config->wolmac[1],
                        &config->wolmac[2],
                        &config->wolmac[3],
                        &config->wolmac[4],
                        &config->wolmac[5]);

    if(result != 6) {

        LOG("Invalid MAC format", FAILED);

        free(read);

        exit(EXIT_FAILURE);
    }

    LOG("Loaded WOL MAC", SUCCESS);

    free(read);
}

void load_all() {

    config = malloc(sizeof(CONFIG));

    if(!config) {

        LOG("Failed to allocate CONFIG", FAILED);

        exit(EXIT_FAILURE);
    }

    memset(config, 0, sizeof(CONFIG));

    load_ip();

    load_broadcastip();

    load_targetip();

    load_port();

    load_targetport();

    load_auth_password();

    load_wolmac();

    LOG("All configs loaded", SUCCESS);
}

CONFIG *global_config() {

    return config;
}