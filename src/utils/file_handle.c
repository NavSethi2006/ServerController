#include "file_handle.h"

void write_file(FILE *fp, const char *string) {
    if (!fp) return;
    fprintf(fp, "%s\n", string);
    fflush(fp);
}

FILE *fp;

FILE *setup_config() {
    fp = fopen("config/config.txt", "a+");
    if(!fp) {
        LOG("Unexpected error occured trying to open config file", FAILED);
        return NULL;
    }
    return fp;
}

void save_file_configs(const char* label, const char *src) {    
    int result = fprintf(fp, "%s:%s\n", label, src);
    if (result < 0) {
        LOG("LINE 23 : Write to config", FAILED);
    } else {
        LOG("Write to config", SUCCESS);
    }
}

char* read_file_configs(const char* label) {
    if (!label) return NULL;

    rewind(fp);

    char read_buffer[256];

    while (fgets(read_buffer, sizeof(read_buffer), fp)) {
        char *colon = strchr(read_buffer, ':');
        if (!colon) continue;

        *colon = '\0';

        if (strcmp(read_buffer, label) == 0) {
            char *value = colon + 1;
            value[strcspn(value, "\n")] = '\0';
            char *result = malloc(strlen(value) + 1);
            if (!result) return NULL;
            strcpy(result, value);
            LOG("Found label in config", SUCCESS);
            return result;
        }
    }

    LOG("Label not found in config", FAILED);
    return NULL;
}

