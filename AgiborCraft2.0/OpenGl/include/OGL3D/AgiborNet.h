#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <ws2tcpip.h>

#include "Packet.h"


#define GAME_PORT 6969


class AgiborNet {
public:
    AgiborNet();
    ~AgiborNet();
    void Send(int type,int id ,float x = 0, float y = 0, float z = 0, float w = 0);
    void Send(Packet packet);
    Packet Recive();

private:
    float a_htonf(float host_float);
    float a_ntohf(float net_float);
    WSADATA wsaData;
    SOCKET send_socket;
    SOCKET recvie_socket;
    sockaddr_in send_ip;
    sockaddr_in broadcast_recive_address;
    sockaddr_in broadcast_send_address;
};