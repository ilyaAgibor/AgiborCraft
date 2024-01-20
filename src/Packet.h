#pragma once

#define BLOCK_DESTRUCTION_PACKET       0
#define BLOCK_PLACEMENT_PACKET         1
#define PLAYER_DIRECTION_PACKET        2
#define PLAYER_POS_PACKET              3
#define PLAYER_CONNECTION_PACKET       4
#define PLAYER_DISCONNECTION_PACKET    5
#define PLAYER_POS_DIRECTION           6


typedef struct _Packet {
    int type;
    int id;
    float x;
    float y;
    float z;
    float w;
}Packet;
