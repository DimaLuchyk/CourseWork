#ifndef SERVER_SHARED_H
#define SERVER_SHARED_H

#include <cstddef>
#include <cstdint>

static const std::byte STARTOFPACKET{0xA7};

enum PacketType
{

};

struct PacketHeader
{
    std::byte startofpacket;
    PacketType packetType;
    std::uint32_t payloadLength;
    //payload
};

#endif //SERVER_SHARED_H
