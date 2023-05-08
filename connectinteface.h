#ifndef CONNECTINTEFACE_H
#define CONNECTINTEFACE_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

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

const char CMD_IDN[] = {"WG0:*IDN?"};
const char CMD_TST[] = {"WG0:*TST?"};
const char CMD_ABOR[] = {"WG0:ABOR"};
const char CMD_INIT[] = {"WG0:INIT"};
const char CMD_UPD[] = {"WG0:UPD"};

class ConnectInteface
{
public:
    ConnectInteface();
    ~ConnectInteface();
    errorConnIntf CheckConnect(char *server_message, int len_ser_msg);
    errorConnIntf Disconnect(void);
    errorConnIntf RunTest(void);
    errorConnIntf Send_command(char *cmd, int len_cmd,char *server_message, int len_ser_msg);
    void Get_Server_IP(uint8_t *IP){ memcpy(IP, SERVER_IP_ADDRESS, sizeof(SERVER_IP_ADDRESS)); };
    void Set_Server_IP(uint8_t *IP){ memcpy(SERVER_IP_ADDRESS, IP, sizeof(SERVER_IP_ADDRESS)); };
private:
    errorConnIntf createSocket(int &socket_desc, sockaddr_in &server_addr, unsigned int &server_struct_length);
    uint8_t SERVER_IP_ADDRESS[4];
};

#endif // CONNECTINTEFACE_H
