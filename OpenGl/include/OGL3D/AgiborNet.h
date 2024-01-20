#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <atomic>

#include "Packet.h"


#define GAME_PORT 6969
#define MAX_MSG_SIZE 4096// 4k

class AgiborNet {
public:
    AgiborNet();
    ~AgiborNet();
    void EncryptPacket(char* packet_buffer, unsigned int size);
    void DecryptPacket(char* packet_buffer, unsigned int size);
    void Send(int type,int id ,float x = 0, float y = 0, float z = 0, float w = 0);
    void Send(Packet packet);
    Packet Recive();
private:
    const char* shared_key = "5UPEr dUp3r s3CR3t kEy!! PlE@se DoNT ShAre";
    size_t keyLength = strlen(shared_key);
    float a_htonf(float host_float);
    float a_ntohf(float net_float);
    WSADATA wsaData;
    SOCKET send_socket;
    SOCKET recvie_socket;
    sockaddr_in send_ip;
    sockaddr_in broadcast_recive_address;
    sockaddr_in broadcast_send_address;
};