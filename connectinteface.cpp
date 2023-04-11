#include "connectinteface.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>





ConnectInteface::ConnectInteface()
{

}


errorConnIntf ConnectInteface::createSocket(int &socket_desc, struct sockaddr_in &server_addr, unsigned int &server_struct_length){

    struct timeval tv;

    server_struct_length = sizeof(server_addr);

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    //time to wait in a comunication
    tv.tv_sec = 0;
    tv.tv_usec = 10000; // 10ms
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

errorConnIntf ConnectInteface::CheckConnect(){

    int socket_desc;
    struct sockaddr_in server_addr;
    unsigned int server_struct_length;

    char server_message[30];
    char client_message[] = "WG0:*IDN?";

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));

    if(createSocket(socket_desc, server_addr, server_struct_length)){
        return ERCI_CREATE_SOCKET;
    }

    // Send the message to server:
    if(sendto(socket_desc, client_message, strlen(client_message), 0,
               (struct sockaddr*)&server_addr, server_struct_length) < 0){

        // Close the socket:
        close(socket_desc);

        return ERCI_SEND_MESSAGE;
    }

    // Receive the server's response:
    if(recvfrom(socket_desc, server_message, sizeof(server_message), 0,
               (struct sockaddr*)&server_addr, &server_struct_length) < 0){

        // Close the socket:
        close(socket_desc);

        return ERCI_RECV_MESSAGE;
    }

    // Close the socket:
    close(socket_desc);

    return NO_ERROR;
}


errorConnIntf ConnectInteface::RunTest(void){

    int socket_desc;
    struct sockaddr_in server_addr;
    unsigned int server_struct_length;

    char server_message[30];
    char client_message[] = "WG0:*IDN?";

    int index;

    for(index=0;index<10;index++){

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));

    if(createSocket(socket_desc, server_addr, server_struct_length)){
        return ERCI_CREATE_SOCKET;
    }

    // Send the message to server:
    if(sendto(socket_desc, client_message, strlen(client_message), 0,
               (struct sockaddr*)&server_addr, server_struct_length) < 0){

        // Close the socket:
        close(socket_desc);

        return ERCI_SEND_MESSAGE;
    }

    // Receive the server's response:
    if(recvfrom(socket_desc, server_message, sizeof(server_message), 0,
                 (struct sockaddr*)&server_addr, &server_struct_length) < 0){

        // Close the socket:
        close(socket_desc);

        return ERCI_RECV_MESSAGE;
    }

    // Close the socket:
    close(socket_desc);



    sleep(1);

    }


    return NO_ERROR;
}

