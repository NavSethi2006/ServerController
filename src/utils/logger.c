#include "logger.h"

FILE *logger_file;
char time_buffer[100];

struct tm* getCurrentDate() {
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    return local;
}

void create_log() {
    struct tm* local = getCurrentDate();
    strftime(time_buffer, sizeof(time_buffer), "logs/%Y-%m-%d.txt", local);   

    logger_file = fopen(time_buffer, "w");
    if (!logger_file) {
        perror("fopen failed");
        return;
    }
    
}

void LOG(char* to_log, STATUS status) {
    struct tm* local = getCurrentDate();
    strftime(time_buffer, sizeof(time_buffer), "[%H:%M:%S]", local);
    char infobuff[1000] = "";
    switch(status) {
        case SUCCESS:
            strcat(infobuff, "[SUCCESS]");
        break;
        case FAILED:
            strcat(infobuff, "[FAILED]");
        break;
        case INFO:
            strcat(infobuff, "[INFO]");
        break;
    }
    fprintf(logger_file,"%s %s %s\n",time_buffer, infobuff, to_log);
    fflush(logger_file);
}