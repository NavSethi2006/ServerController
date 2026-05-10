#include "server.h"

#define MAX_CONNECTION 5

int serverfd, client_socket;
struct sockaddr_in server_address, client_addr;
int addrlen;
pthread_t server_thread;
char ip[INET_ADDRSTRLEN];

volatile __sig_atomic_t shutdown_flag = 0;

void handle_sigint(int signum) {
    shutdown_flag = 1;
}

void setup_server() {
    signal(SIGINT, handle_sigint);

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverfd < 0) {
        LOG("CRITICAL LINE 13 server.c: Server creation, Exiting....", FAILED);
        exit(EXIT_FAILURE);
    } else {
        LOG("Server creation", SUCCESS);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = global_config()->ip;
    server_address.sin_port = htons(global_config()->port);

    LOG("Server details have been populated", INFO);

    if(bind(serverfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        LOG("LINE 24 server.c: Could not bind to socket to ip and port in config, Exiting....", FAILED);
        close(serverfd);
        exit(EXIT_FAILURE);
    }
    LOG("Server binded", SUCCESS);
    if (listen(serverfd, 3) < 0) {
        LOG("LINE 30 server.c: Could not listen to port ", FAILED);
        close(serverfd);
        exit(EXIT_FAILURE);
    }
    LOG("Server listening on port ", INFO);
    LOG("Server is ready", INFO);
    addrlen = sizeof(server_address);
    while(!shutdown_flag) {
        client_socket = accept(serverfd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen);
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
        char infobuff[100];

        sprintf(infobuff, "Client at %s is trying to connect to the server, sending authentication", ip);
        LOG(infobuff, INFO);
        if(auth_handler(client_socket) == 0) {
            sprintf(infobuff, "Client at %s has failed authentication", ip);
            LOG(infobuff, INFO);
            close(client_socket);
            continue;
        }
        handle_client();
        if(client_socket < 0) {
            LOG("CRITICAL : client could not connect", FAILED);
            continue;
        }
    }
    close(serverfd);
}

void handle_client() {
    char infobuff[31+32];
    sprintf(infobuff, "Accepted client at ip:%s \n", ip);
    LOG(infobuff, INFO);

    bool client_listen = true;

    char client_buffer[10]; 
    while(client_listen) {

        int result = recv(client_socket, client_buffer, sizeof(client_buffer) - 1, 0);
        if(result < 0) {
            sprintf(infobuff, "Client at %s has left abruptly", ip);
            LOG(infobuff, INFO);
            client_listen = false;
            break;
        } else {
            sprintf(infobuff,"Recived server instruction from client. Instruction being : %s", client_buffer);
            int count = sizeof(commands) / sizeof(Command);
            printf("%s\n", client_buffer);
            for(int i = 0; i < count; i++) {
                if(strcmp(client_buffer, commands[i].name) == 0) {
                    commands[i].func(client_socket);
                } else {
                    send(client_socket, "UNKNOWN COMMAND\n", 16, 0);
                }
            }

        }

    }

    close(client_socket);
    
}
