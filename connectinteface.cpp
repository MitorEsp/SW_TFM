#include "connectinteface.h"
#include <stdio.h>

#include <sys/time.h>
#include <unistd.h>

ConnectInteface::ConnectInteface() {
  SERVER_IP_ADDRESS[0] = 16;
  SERVER_IP_ADDRESS[1] = 0;
  SERVER_IP_ADDRESS[2] = 0;
  SERVER_IP_ADDRESS[3] = 16;
}

ConnectInteface::~ConnectInteface() {}

errorConnIntf ConnectInteface::Send_command(char *cmd, int len_cmd,
                                            char *server_message,
                                            int len_ser_msg) {

  int socket_desc;
  struct sockaddr_in server_addr_out;
  struct sockaddr_in server_addr_in;
  unsigned int server_struct_length;
  struct timeval tv;

  // Clean buffers:
  memset(server_message, '\0', len_ser_msg);

  server_struct_length = sizeof(server_addr_out);

  // Create socket:
  socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  // time to wait in a comunication
  tv.tv_sec = 0;
  tv.tv_usec = 200000; // 5ms
  setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  setsockopt(socket_desc, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

  if (socket_desc < 0) {
    return ERCI_CREATE_SOCKET;
  }

  // Set port and IP:
  server_addr_out.sin_family = AF_INET;
  server_addr_out.sin_port = htons(64000);
  server_addr_out.sin_addr.s_addr = inet_addr("16.0.0.16");

  // Set port and IP:
  server_addr_in.sin_family = AF_INET;
  server_addr_in.sin_port = htons(65000);
  server_addr_in.sin_addr.s_addr = inet_addr("16.0.0.10");

  int errorBind = bind(socket_desc, (struct sockaddr *)&server_addr_in,
                       sizeof(server_addr_in));

  if (errorBind < 0) {
    return ERCI_BIND;
  }

  // Send the message to server:
  if (sendto(socket_desc, cmd, len_cmd, 0, (struct sockaddr *)&server_addr_out,
             server_struct_length) < 0) {

    // Close the socket:
    close(socket_desc);

    return ERCI_SEND_MESSAGE;
  }

  // Receive the server's response:
  if (recvfrom(socket_desc, server_message, len_ser_msg, 0,
               (struct sockaddr *)&server_addr_in, &server_struct_length) < 0) {

    // Close the socket:
    close(socket_desc);

    return ERCI_RECV_MESSAGE;
  }

  // Close the socket:
  close(socket_desc);

  return NO_ERROR;
}

errorConnIntf ConnectInteface::Receive_command(char *server_message,
                                               int len_ser_msg) {

  int socket_desc;
  struct sockaddr_in server_addr_out;
  struct sockaddr_in server_addr_in;
  unsigned int server_struct_length;
  struct timeval tv;

  // Clean buffers:
  memset(server_message, '\0', len_ser_msg);

  server_struct_length = sizeof(server_addr_out);

  // Create socket:
  socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  // time to wait in a comunication
  tv.tv_sec = 1;
  tv.tv_usec = 0; // 5ms
  setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  setsockopt(socket_desc, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

  if (socket_desc < 0) {
    return ERCI_CREATE_SOCKET;
  }

  // Set port and IP:
  server_addr_out.sin_family = AF_INET;
  server_addr_out.sin_port = htons(64000);
  server_addr_out.sin_addr.s_addr = inet_addr("16.0.0.16");

  // Set port and IP:
  server_addr_in.sin_family = AF_INET;
  server_addr_in.sin_port = htons(65000);
  server_addr_in.sin_addr.s_addr = inet_addr("16.0.0.10");

  int errorBind = bind(socket_desc, (struct sockaddr *)&server_addr_in,
                       sizeof(server_addr_in));

  if (errorBind < 0) {
    return ERCI_BIND;
  }

  // Receive the server's response:
  if (recvfrom(socket_desc, server_message, len_ser_msg, 0,
               (struct sockaddr *)&server_addr_in, &server_struct_length) < 0) {

    // Close the socket:
    close(socket_desc);

    return ERCI_RECV_MESSAGE;
  }

  // Close the socket:
  close(socket_desc);

  return NO_ERROR;
}
