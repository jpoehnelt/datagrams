//
// Created by Justin on 11/12/15.
//

#include "server.h"

void run();

int main() {
    // create the server
    run();

    // this line should never execute
    return 1;
}


void run() {
    // declare server socket and server address struct
    struct sockaddr_in server_address, client_address;
    int server_socket, byte_count;
    socklen_t send_length = sizeof(client_address);
    char buffer[BUFFER_SIZE];


    // create the socket -- TCP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        printf("Failed to create server socket");
        exit(1);
    }

    // set necessary connection information
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);

    // Bind Port to Socket
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        printf("Failed to bind server socket");
        exit(1);
    }

    while (1) {

        printf("Waiting for data on port: %d \n", PORT);

        // receive some data,blocks
        if ((byte_count = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_address,
                                   &send_length)) == -1) {
            printf("Failed to receive from socket.");
            exit(1);
        }

        if (sendto(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &client_address, send_length) == -1) {
            printf("Failed to send datagram.");
            exit(1);
        }

        printf("Received packet from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        printf("Data: %s\n" , buffer);

    }
}
