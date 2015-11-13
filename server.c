#include "server.h"

void run();

char *get_time();
char *lookup_ip(char * url);
int send_datagram(int socket, struct datagram packet_out, int size, struct sockaddr_in address, int len);

int main() {
    run();
    // this line should never execute
    return 1;
}

/**
 * Waits for datagrams and responds as appropriate.
 */
void run() {
    // declare server socket and server address struct
    struct sockaddr_in server_address, client_address;
    int server_socket, byte_count;
    socklen_t send_length = sizeof(client_address);
    char buffer[BUFFER_SIZE];
    struct datagram packet_in, packet_out;

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
        struct datagram packet_out;

        printf("\n\n*** Waiting for data on port: %d ***\n\n", PORT);

        // receive some data, blocks while waiting
        if ((byte_count = recvfrom(server_socket, &packet_in, BUFFER_SIZE, 0, (struct sockaddr *) &client_address,
                                   &send_length)) == -1) {
            printf("Failed to receive from socket.");
            exit(1);
        }

        printf("Received packet from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        printf("Packet Type: %d\n", packet_in.type);
        printf("Data: %s\n", packet_in.data);


        // switch on the packet type, response packet type is always + 1
        switch (packet_in.type) {
            case 1: // echo
                packet_out.type = packet_in.type + 1;;
                strcpy(packet_out.data, packet_in.data);
                send_datagram(server_socket, packet_out, BUFFER_SIZE, client_address, send_length);
                break;
            case 3: // time
                packet_out.type = packet_in.type + 1;;
                strcpy(packet_out.data, get_time());
                send_datagram(server_socket, packet_out, BUFFER_SIZE, client_address, send_length);
                break;
            case 5: // ip lookup
                packet_out.type = packet_in.type + 1;
//                lookup_ip(packet_in.data);
                strcpy(packet_out.data, lookup_ip(packet_in.data));
                send_datagram(server_socket, packet_out, BUFFER_SIZE, client_address, send_length);
                break;
            default:
                packet_out.type = 0;;
                strcpy(packet_out.data, "Do not understand!");
                send_datagram(server_socket, packet_out, BUFFER_SIZE, client_address, send_length);
        }
    }
}

/*
 * Send a datagram to an address.
 */
int send_datagram(int socket, struct datagram packet_out, int size, struct sockaddr_in address, int len) {
    if (sendto(socket, &packet_out, size, 0, (struct sockaddr *) &address, len) == -1) {
        printf("Failed to send datagram.");
        return (1);
    }

    printf("\nSent packet to %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
    printf("Packet Type: %d\n", packet_out.type);
    printf("Data: %s\n", packet_out.data);
    return 0;
}

/*
 * Get current local time string.
 */
char *get_time() {
    time_t rawtime;
    struct tm *info;
    static char buffer[80];

    time(&rawtime);

    info = localtime(&rawtime);

    strftime(buffer, 80, "%H:%M", info);
//    printf("Formatted date & time : |%s|\n", buffer);
    return buffer;
}

/*
 * Lookup up and return an ip for a url.
 */
char *lookup_ip(char * url) {
    int status;
    struct addrinfo hints, *results;  // will point to the results
    void *addr;
    char *ipver;
    static char ipstr[INET6_ADDRSTRLEN];
    struct sockaddr_in *address;

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if ((status = getaddrinfo(url, "80", &hints, &results)) != 0) {
        printf("Error get ip address: %s\n", gai_strerror(status));
    }

    // TODO ugly refactor
    inet_ntop(results->ai_family, &((struct sockaddr_in *)results->ai_addr)->sin_addr, ipstr, sizeof ipstr);

    return ipstr;
}