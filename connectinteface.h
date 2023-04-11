#ifndef CONNECTINTEFACE_H
#define CONNECTINTEFACE_H

#include <arpa/inet.h>
#include <sys/socket.h>

typedef enum{
    NO_ERROR            = (int)0,
    ERCI_CREATE_SOCKET  = (int)1,   /* Error while creating socket */
    ERCI_SEND_MESSAGE   = (int)2,   /* Unable to send message */
    ERCI_RECV_MESSAGE   = (int)3   /* Unable to receive message */
} errorConnIntf;

const char stringErrorConnIntf[3][30]={
    "Error while creating socket",
    "Unable to send message",
    "Unable to receive message"};

class ConnectInteface
{
public:
    ConnectInteface();
    errorConnIntf CheckConnect(void);
    errorConnIntf Disconnect(void);
    errorConnIntf RunTest(void);
private:

    errorConnIntf createSocket(int &socket_desc, sockaddr_in &server_addr, unsigned int &server_struct_length);
};

#endif // CONNECTINTEFACE_H
