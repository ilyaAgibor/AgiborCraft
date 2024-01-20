#include "OGL3D/AgiborNet.h"

#define MAX_CHUNK_SIZE sizeof(int)

float AgiborNet::a_htonf(float host_float) {
    unsigned int net_bytes;
    memcpy(&net_bytes, &host_float, sizeof(float));
    net_bytes = htonl(net_bytes);
    memcpy(&host_float, &net_bytes, sizeof(float));
    return host_float;
}

float AgiborNet::a_ntohf(float net_float) {
    unsigned int host_bytes;
    memcpy(&host_bytes, &net_float, sizeof(float));
    host_bytes = ntohl(host_bytes);
    memcpy(&net_float, &host_bytes, sizeof(float));
    return net_float;
}

AgiborNet::AgiborNet() {
    int err = WSAStartup(MAKEWORD(2, 2), &this->wsaData);
    if (err != 0) {
        std::cerr << "WSAStartup failed with error: " << err << std::endl;
    }

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        printf("gethostname failed with error: %d\n", WSAGetLastError());
        WSACleanup();
    }
    struct addrinfo* local_ip = NULL;
    struct addrinfo hints;
    
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    
    int iResult = getaddrinfo(hostname, NULL, &hints, &local_ip);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
    }

    char ip[INET6_ADDRSTRLEN];
    struct sockaddr_in* sockaddr_ipv4;
    struct sockaddr_in6* sockaddr_ipv6;

    int i = 0;
    std::vector<sockaddr_in> local_ipv4s;
    printf("Choose the ip you want to use as your local ip for the game: \n");
    for (struct addrinfo* ptr = local_ip; ptr != NULL; ptr = ptr->ai_next) {
        if (ptr->ai_family == AF_INET) {
            sockaddr_ipv4 = (struct sockaddr_in*)ptr->ai_addr;
            inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ip, sizeof(ip));
            local_ipv4s.push_back(*sockaddr_ipv4);
            printf("%d. Local IP address: %s\n",i,ip);
            i++;
        }
    }
    freeaddrinfo(local_ip);

    int index = 0;
    char in = fgetc(stdin);
    in -= 48;
    if (in >= local_ipv4s.size() || in < 0)
        in = 0;

    ZeroMemory(&this->send_ip, sizeof(this->send_ip));
    this->send_ip.sin_family = AF_INET;
    this->send_ip.sin_port = htons(0);
    this->send_ip.sin_addr.s_addr = local_ipv4s[in].sin_addr.s_addr; // set the desired local IP address


    this->send_socket= socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (send_socket == INVALID_SOCKET) {
        std::cerr << "send socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
    }
    this->broadcast_send_address.sin_family = AF_INET;
    this->broadcast_send_address.sin_port = htons(GAME_PORT);
    this->broadcast_send_address.sin_addr.s_addr = inet_addr("255.255.255.255");
    bind(send_socket, (SOCKADDR*)&this->send_ip, sizeof(this->send_ip));

    int result = setsockopt(this->send_socket, SOL_SOCKET, SO_BROADCAST, (char*)&this->broadcast_send_address, sizeof(true));
    if (result == SOCKET_ERROR) {
        std::cerr << "setsockopt send broadcast failed: " << WSAGetLastError() << std::endl;
        closesocket(this->send_socket);
        WSACleanup();
    }
    int bufsize = MAX_MSG_SIZE; // 4k
    result = setsockopt(this->send_socket, SOL_SOCKET, SO_SNDBUF, (const char*)&bufsize, sizeof(int));
    if (result == SOCKET_ERROR) {
        std::cerr << "setsockopt send max size failed: " << WSAGetLastError() << std::endl;
        closesocket(this->send_socket);
        WSACleanup();
    }

    this->recvie_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (this->recvie_socket == INVALID_SOCKET) {
        std::cerr << "recive socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
    }
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    result = setsockopt(this->recvie_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    if (result < 0) {
        std::cerr << "setsockopt failed: " << WSAGetLastError() << std::endl;
        closesocket(this->recvie_socket);
        WSACleanup();
    }
    this->broadcast_recive_address.sin_family = AF_INET;
    this->broadcast_recive_address.sin_addr.s_addr = INADDR_ANY;
    this->broadcast_recive_address.sin_port = htons(GAME_PORT);
    int a = bind(this->recvie_socket, (struct sockaddr*)&this->broadcast_recive_address, sizeof(this->broadcast_recive_address));
    if (a == -1) {
        std::cerr << "error binding recive socket" << std::endl;
        WSACleanup();
    }
    BOOL broadcast = TRUE;
    setsockopt(this->recvie_socket, SOL_SOCKET, SO_BROADCAST, (char*)&this->broadcast_recive_address, sizeof(this->broadcast_recive_address));
}

AgiborNet::~AgiborNet() {
    /*if(target_ip != nullptr)
        delete[] target_ip;*/
    closesocket(this->send_socket);
    WSACleanup();
}

void AgiborNet::EncryptPacket(char* packet_buffer, unsigned int size) {
    // XOR each byte of the packet with the corresponding byte of the key
    unsigned char* packetBytes = (unsigned char*)packet_buffer;
    for (unsigned int i = 0; i < size; ++i) {
        packetBytes[i] ^= shared_key[i % keyLength];
    }
}

void AgiborNet::DecryptPacket(char* packet_buffer, unsigned int size) {
    // Decrypt the packet using XOR with the key
    EncryptPacket(packet_buffer, size);
}

void AgiborNet::Send(int type,int id, float x, float y, float z, float w)
{
    Packet packet{ 0 };
    packet.type = htonl(type);
    char buffer[sizeof(packet)+13] = "AgiborCraft "; 
    packet.id = htonl(id);
    packet.x = a_htonf(x);
    packet.y = a_htonf(y);
    packet.z = a_htonf(z);
    packet.w = a_htonf(w);
    memcpy(buffer + 12, &packet, sizeof(packet));
    EncryptPacket(buffer, sizeof(buffer));
    int iResult = sendto(this->send_socket, (const char*)buffer, sizeof(buffer), 0, (sockaddr*)&this->broadcast_send_address, sizeof(this->broadcast_send_address));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
    }
}


void AgiborNet::Send(Packet packet) {
    packet.type = htonl(packet.type);
    char buffer[sizeof(packet) + 13] = "AgiborCraft ";  //checksum: 0x464
    packet.id = htonl(packet.id);
    packet.x = a_htonf(packet.x);
    packet.y = a_htonf(packet.y);
    packet.z = a_htonf(packet.z);
    packet.w = a_htonf(packet.w);
    memcpy(buffer + 12, &packet, sizeof(packet));
    EncryptPacket(buffer, sizeof(buffer));
    int iResult = sendto(this->send_socket, (const char*)buffer, sizeof(buffer), 0, (sockaddr*)&this->broadcast_send_address, sizeof(this->broadcast_send_address));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
    }
}

Packet AgiborNet::Recive()
{
    Packet packet{ 0 };
    char rec_buffer[sizeof(Packet) + 13];
    struct sockaddr_in sender_addr;
    int sender_addr_size = sizeof(sender_addr);
    int recv_size = recvfrom(this->recvie_socket, rec_buffer, sizeof(rec_buffer), 0, (struct sockaddr*)&sender_addr, &sender_addr_size);
    if (sender_addr.sin_addr.s_addr == this->send_ip.sin_addr.s_addr || recv_size <= 0) {
        Packet empty_packet = { 0 };
        empty_packet.type = -1;
        return empty_packet;
    }
    DecryptPacket(rec_buffer, sizeof(rec_buffer));
    memcpy(&packet, rec_buffer + 12, sizeof(Packet));
    packet.type = ntohl(packet.type);
    packet.id = ntohl(packet.id);
    packet.x = a_ntohf(packet.x);
    packet.y = a_ntohf(packet.y);
    packet.z = a_ntohf(packet.z);
    packet.w = a_ntohf(packet.w);
    //printf("recived packet of type %d from ip:%s that contains:\nx:%f\ny:%f\nz:%f\nw:%f\nchecksum:%x\n", packet.type, inet_ntoa(sender_addr.sin_addr),packet.x, packet.y, packet.z, packet.w, packet.checksum);
    return packet;
}

