#include "security.h"

void secure_random_bytes(unsigned char *output, size_t size) {
    RAND_bytes(output, size);
}

// Generate a secure challenge
void generate_challenge(char *buffer, size_t size) {
    const char *chars = "abcdefghijklmnopqrstuvwxyz0123456789";
    size_t len = strlen(chars);
    secure_random_bytes(buffer, size);
    for (size_t i = 0; i < size - 1; i++) {
        buffer[i] = chars[buffer[i] % len];
    }
    buffer[size - 1] = '\0';
}

// Compute SHA-256 hash
void sha256(const char *input, char output[65]) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input, strlen(input), hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = '\0';
}

int auth_handler(int client_socket) {
    char challenge[32];
    char buffer[256];

    // Generate a secure challenge
    generate_challenge(challenge, sizeof(challenge));

    LOG("Sending challenge to connected client", INFO);

    sprintf(buffer, "CHALLENGE %s\n", challenge);
    send(client_socket, buffer, strlen(buffer), 0);
    LOG("Receiving response", INFO);

    int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) {
        LOG("Response not received, kicking client", INFO);
        return 0;
    }

    buffer[bytes] = '\0';
    buffer[strcspn(buffer, "\r\n")] = '\0';

    char *received_hash = strchr(buffer, ' ');
    if (!received_hash) {
        LOG("Invalid response format, kicking client", INFO);
        return 0;
    }

    received_hash++;
    received_hash[strcspn(received_hash, "\n")] = '\0';

    char combined[128];
    sprintf(combined, "%s%s", global_config()->auth_password, challenge);
    char expected_hash[65];
    sha256(combined, expected_hash);

    LOG("Computed expected hash: %s", INFO);
    printf("Expected hash: %s\n", expected_hash);

    if (strcmp(received_hash, expected_hash) == 0) {
        LOG("Authentication successful", SUCCESS);
        send(client_socket, "OK\n", 3, 0);
        return 1;
    }

    LOG("Authentication failed", INFO);
    send(client_socket, "FAIL\n", 5, 0);
    return 0;
}