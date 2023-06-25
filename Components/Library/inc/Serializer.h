#ifndef COURSEWORK_SERIALIZER_H
#define COURSEWORK_SERIALIZER_H

#include "Packet.h"

#include <utility>

namespace coursework::protocol
{
    class Serializer
    {
    public:
        static QByteArray combineToPacket(const PacketHeader& header, const Payload& payload);

        static QByteArray combineToPacket(const PacketHeader& header, const FilePaylaod& payload);

        static QByteArray combineToPacket(const PacketHeader& header, const AuthorizationPayload& payload);

        static Payload toPayload(QByteArray& data);

        static FilePaylaod toFilePayload(QByteArray& data);

        static void toAuthorizationPayload(QByteArray& data, PacketHeader& outHeader, AuthorizationPayload& outPayload);

    private:
        //static PacketHeader* m_header; //will be used for internal convertions
    };

}

#endif //COURSEWORK_SERIALIZER_H
