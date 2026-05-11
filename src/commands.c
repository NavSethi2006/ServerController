#include "commands.h"

void cmd_start(int client) {
}

void build_wol_packet(unsigned char *packet) {
        // build magic packet #FF
    for(int i = 0; i < 6; i++)
        packet[i] = 0XFF;
    for(int i = 0;i < 16; i++)
        memcpy(packet+6+(i*6), global_config()->wolmac, 6);

    LOG("Magic packet created", INFO);

}

// build magic packet and send it to the server to wake up
void send_wol(int client) {

    LOG("Creating datagram packet", INFO);
    
    int sock;
    struct sockaddr_in addr;
    unsigned char packet[102];

    build_wol_packet(packet);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) {
        LOG("CRITICAL (Line 31 commands.c): cannot creating socket for datagram", FAILED);
    }

    int broadcast = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(9);
    addr.sin_addr.s_addr = global_config()->broadcastip;

    LOG("Server address populated, sending packet...", INFO);
    
    int result = sendto(sock, packet, sizeof(packet), 0, (struct sockaddr*)&addr, sizeof(addr));
    if(result < 0) {
        LOG("CRITICAL (Line 44 commands.c): cannot send packet to local client", FAILED);
        close(sock);
    }
    LOG("The packet has been sent to the server wait 6 seconds then ping", SUCCESS);
    close(sock);
}

void cmd_stop(int client) {

    LOG("Command sent to shutdown the computer", INFO);
    int sock;
    struct sockaddr_in addr;
    char stopsig[4] = "STOP";

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        LOG("CRITICAL LINE 60 commands.c: Server creation, Exiting....", FAILED);
        exit(EXIT_FAILURE);
    } else {
        LOG("Command for send signal created", SUCCESS);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = global_config()->targetip;
    addr.sin_port = htons(global_config()->targetport);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        LOG("Target server is offline, cannot connect", FAILED);
        send(client, "1", 1, 0);  // tell python side it failed
        close(sock);
        return;
    }
    LOG("Communication being esablished for shutdown signal", INFO);
    
    LOG("Command ready to be sent ", INFO);
    send(sock, stopsig, sizeof(stopsig), 0);
    LOG("Command STOPSIG SENT", SUCCESS);
    char recvbuf[2] = {0};

    int bytes = recv(sock, recvbuf, 1, 0);

    if(bytes <= 0) {
        LOG("Failed to receive response from shutdown server", FAILED);
        send(client, "1", 1, 0);
        close(sock);
        return;
    }

    if(recvbuf[0] == '0') {
        LOG("Server turned off successfully", SUCCESS);
        send(client, "0", 1, 0);
    }
    
    close(sock);

}
