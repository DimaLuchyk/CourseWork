#ifndef COURSEWORK_PACKETGENERATOR_H
#define COURSEWORK_PACKETGENERATOR_H

#include "Packet.h"

namespace coursework::protocol
{

    class PacketGenerator
    {
    public:
        static PacketHeader generatePacketHeader(const PacketType packetType, const std::uint32_t payloadLength);

    private:
        static QString generateTimeStamp();
    };

}

#endif //COURSEWORK_PACKETGENERATOR_H
