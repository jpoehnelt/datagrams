//
// Created by Justin on 11/12/15.
//

#include "client.h"


int main() {
    // declare server socket and server address struct
    struct sockaddr_in client_address, server_address;
    int client_socket, byte_count;
    socklen_t send_length = sizeof(server_address);
    char buffer[] = "Hello World!";


    // create the socket -- TCP socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        printf("Failed to create server socket");
        exit(1);
    }

    // set necessary connection information
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_address.sin_port = htons(PORT_CLIENT);

    // set necessary connection information
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT_SERVER);

    // Bind Port to Socket
    if (bind(client_socket, (struct sockaddr *) &client_address, sizeof(client_address)) == -1) {
        printf("Failed to bind server socket");
        exit(1);
    }

    if (sendto(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &server_address, send_length) == -1) {
        printf("Failed to send datagram.");
        exit(1);
    }

    // Block while waiting to receive datagram.
    if ((byte_count = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &server_address, &send_length)) == -1) {
        printf("Failed to receive datagram.");
        exit(1);
    }
    
    
    printf("Received packet from %s:%d\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
    printf("Data: %s\n" , buffer);


    // this line should never execute
    return 1;
}

