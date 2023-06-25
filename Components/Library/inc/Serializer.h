#ifndef COURSEWORK_SERIALIZER_H
#define COURSEWORK_SERIALIZER_H

#include "Packet.h"

namespace coursework::protocol
{
    class Serializer
    {
    public:
        static QByteArray combineToPacket(const PacketHeader& header, const Payload& payload);

        static QByteArray combineToPacket(const PacketHeader& header, const FilePaylaod& payload);

        static QByteArray combineToPacket(const PacketHeader& header, const AuthorizationPayload& payload);

        static void toPayload(QByteArray& data, PacketHeader& outHeader, Payload& outPayload);

        static void toFilePayload(QByteArray& data, PacketHeader& outHeader, FilePaylaod& outPayload);

        static void toAuthorizationPayload(QByteArray& data, PacketHeader& outHeader, AuthorizationPayload& outPayload);
    };

}

#endif //COURSEWORK_SERIALIZER_H
