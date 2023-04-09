#include "connectinteface.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>


ConnectInteface::ConnectInteface()
{

}


bool ConnectInteface::CheckConnect(){

    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[100], client_message[100];
    char init_message[] = "Hola";
    unsigned int server_struct_length = sizeof(server_addr);
    struct timeval tv;

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    //time to wait in a comunication
    tv.tv_sec = 0;
    tv.tv_usec = 10000; // 10ms
    setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(socket_desc, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    if(socket_desc < 0){
      //  printf("Error while creating socket\n");
        return false;
    }
 //   printf("Socket created successfully\n");

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7);
    server_addr.sin_addr.s_addr = inet_addr("16.0.0.16");

    // Send the message to server:
    if(sendto(socket_desc, init_message, strlen(init_message), 0,
               (struct sockaddr*)&server_addr, server_struct_length) < 0){
  //      printf("Unable to send message\n");
        return false;
    }

    // Receive the server's response:
    if(recvfrom(socket_desc, server_message, sizeof(server_message), 0, (struct sockaddr*)&server_addr, &server_struct_length) < 0){
 //       printf("Error while receiving server's msg\n");
        return false;
    }

    printf("Server's response: %s\n", server_message);

    // Close the socket:
    //close(socket_desc);

    return true;
}


