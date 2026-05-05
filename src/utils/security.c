#include "security.h"

void sha256(const char *input, char output[65]) {

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input, strlen(input), hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = '\0';
}

void generate_challange(char* buffer, size_t size) {
    const char *chars = "abcdefghijklmnopqrstuvwxyz0123456789";
    srand(time(NULL));
    for (size_t i = 0; i < size - 1; i++) {
        buffer[i] = chars[rand() % 36];
    }
    buffer[size - 1] = '\0';
}

int auth_handler(int client_socket) {

    char challenge[32];
    char buffer[256];

    generate_challange(challenge, sizeof(challenge));

    LOG("Sending challange to connected client", INFO);
    sprintf(buffer, "CHALLENGE %s\n", challenge);
    send(client_socket, buffer, strlen(buffer), 0);

    LOG("Reciveing response", INFO);
    int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) {
        LOG("Response not sent kicking client", INFO);
        return 0;
    }

    buffer[bytes] = '\0';
    buffer[strcspn(buffer, "\r\n")] = '\0'; 

    char *received_hash = strchr(buffer, ' ');
    if (!received_hash)  {
        LOG("Wrong AUTH kicking client", INFO);
        return 0;
    }
    received_hash++;
    received_hash[strcspn(received_hash, "\n")] = '\0';

    char combined[128];
    sprintf(combined, "%s%s", global_config()->auth_password, challenge);

    char expected_hash[65];
    sha256(combined, expected_hash);

    LOG("Computed expected HASH", INFO);


    if (strcmp(received_hash, expected_hash) == 0) {
        LOG("Auth was correct, letting client in", SUCCESS);
        send(client_socket, "OK\n", 3, 0);
        return 1;
    } else {
        LOG("Auth was incorrect, kicking client out", INFO);
        send(client_socket, "FAIL\n", 5, 0);
        return 0;
    }
}