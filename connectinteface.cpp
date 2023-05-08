#include "connectinteface.h"
#include <stdio.h>

#include <unistd.h>
#include <sys/time.h>


ConnectInteface::ConnectInteface()
{
    SERVER_IP_ADDRESS[0] = 16;
    SERVER_IP_ADDRESS[1] = 0;
    SERVER_IP_ADDRESS[2] = 0;
    SERVER_IP_ADDRESS[3] = 16;
}

ConnectInteface::~ConnectInteface()
{

}

errorConnIntf ConnectInteface::createSocket(int &socket_desc, struct sockaddr_in &server_addr, unsigned int &server_struct_length){

    struct timeval tv;

    server_struct_length = sizeof(server_addr);

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    //time to wait in a comunication
    tv.tv_sec = 0;
    tv.tv_usec = 100000; // 10ms
    setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(socket_desc, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    if(socket_desc < 0){
        return ERCI_CREATE_SOCKET;
    }

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7);
    server_addr.sin_addr.s_addr = inet_addr("16.0.0.16");

    return NO_ERROR;
}



errorConnIntf ConnectInteface::Send_command(char *cmd, int len_cmd, char *server_message, int len_ser_msg){

    int socket_desc;
    struct sockaddr_in server_addr;
    unsigned int server_struct_length;

    // Clean buffers:
    memset(server_message, '\0', len_ser_msg);

    if(createSocket(socket_desc, server_addr, server_struct_length)){
        return ERCI_CREATE_SOCKET;
    }

    // Send the message to server:
    if(sendto(socket_desc, cmd, len_cmd, 0,
               (struct sockaddr*)&server_addr, server_struct_length) < 0){

        // Close the socket:
        close(socket_desc);

        return ERCI_SEND_MESSAGE;
    }

    // Receive the server's response:
    if(recvfrom(socket_desc, server_message, len_ser_msg, 0,
                 (struct sockaddr*)&server_addr, &server_struct_length) < 0){

        // Close the socket:
        close(socket_desc);

        return ERCI_RECV_MESSAGE;
    }

    // Close the socket:
    close(socket_desc);


    return NO_ERROR;
}





