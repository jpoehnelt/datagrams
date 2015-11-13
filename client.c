
#include "client.h"


int main(int argc, char *argv[]) {
    // declare server socket and server address struct
    struct sockaddr_in client_address, server_address;
    int client_socket, byte_count;
    socklen_t send_length = sizeof(server_address);
    char buffer[BUFFER_SIZE];
    struct datagram packet_out, packet_in;


    // check if user passed commands
    if (argc < 2) {
        printf("Please enter a command, packet type followed by optional string.\n");
        return 1;
    }

    // determine service to use
    if (strcmp(argv[1], "echo") == 0) {
        packet_out.type = 1;

        if (argc < 3) {
            printf("Please enter a string to echo!\n");
            return 1;
        } else {
            strcpy(packet_out.data, argv[2]);
        }
    }
    else if (strcmp(argv[1], "time") == 0) {
        packet_out.type = 3;
    }
    else if (strcmp(argv[1], "ip") == 0) {

        packet_out.type = 5;
        if (argc < 3) {
            printf("Please enter an url to lookup!\n");
            return 1;
        } else {
            strcpy(packet_out.data, argv[2]);
        }
    }
    else {
        printf("Please enter a command('echo', 'time', 'ip')\n");
        return 1;
    }


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

//    memcpy(buffer, &data, BUFFER_SIZE);

    if (sendto(client_socket, &packet_out, BUFFER_SIZE, 0, (struct sockaddr *) &server_address, send_length) == -1) {
        printf("Failed to send datagram.");
        exit(1);
    }

    printf("\nSent packet to %s:%d\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
    printf("Packet Type: %d\n", packet_out.type);
    printf("Data: %s\n", packet_out.data);

    // Block while waiting to receive datagram.
    if ((byte_count = recvfrom(client_socket, &packet_in, BUFFER_SIZE, 0, (struct sockaddr *) &server_address,
                               &send_length)) == -1) {
        printf("Failed to receive datagram.");
        exit(1);
    }

    printf("\nReceived packet from %s:%d\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
    printf("Packet Type: %d\n", packet_in.type);
    printf("Data: %s\n", packet_in.data);


    // this line should never execute
    return 1;
}

