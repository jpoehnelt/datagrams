// Common definition for server and client

#ifndef DATAGRAMS_SETTINGS_H
#define DATAGRAMS_SETTINGS_H

#define DATA_SIZE 500

/*
 *  Packet Protocol
 *
 *  Types:
 *  1 - echo query
 *  2 - echo response
 *  3 - time query
 *  4 - time response
 *  5 - ip query
 *  6 - ip response
 *
 *  Data: Optional
 *
 */

struct datagram{
    int type;
    char data[DATA_SIZE];
};

#define BUFFER_SIZE (sizeof(int) + DATA_SIZE)

#endif //DATAGRAMS_SETTINGS_H
