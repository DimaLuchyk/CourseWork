#ifndef SERVER_SHARED_H
#define SERVER_SHARED_H

#include <cstddef>
#include <cstdint>

#include <QString>
#include <QDateTime>
#include <QByteArray>
#include <QDataStream>

namespace coursework::protocol
{

    static const std::byte STARTOFPACKET{0xA7};

    enum PacketType
    {
        //log in & log up
        LOG_IN,
        LOG_IN_SUCCESS,
        LOG_IN_FAILURE,

        LOG_UP,
        LOG_UP_SUCCESS,
        LOG_UP_FAILURE,

        //base communication
        GET_EXISTED_FILES,
        GET_EXISTED_FILES_SUCCESS,
        GET_EXISTED_FILES_FAILURE,

        ADD_FILE,
        ADD_FILE_SUCCESS,
        ADD_FILE_FAILURE,

        REMOVE_FILE,
        REMOVE_FILE_SUCCESS,
        REMOVE_FILE_FAILURE,

        DOWNLOAD_FILE,
        DOWNLOAD_FILE_SUCCESS,
        DOWNLOAD_FILE_FAILURE
    };

    struct PacketHeader
    {
        std::byte startofpacket;            //indicates the start of packet
        std::uint16_t sequenceNumber;            //sequence number
        PacketType packetType;              //packet type
        QString timeStamp;                  //time stamp
        std::uint32_t payloadLength;        //the length of the payload
    };

    struct AuthorizationPayload
    {
        QString username;
        QString password;
    };

    struct Payload
    {
        QByteArray payload;
    };


    class PacketGenerator
    {
    public:
        static PacketHeader generatePacketHeader(const PacketType packetType, const std::uint32_t payloadLength)
        {
            return {STARTOFPACKET, 0, packetType, generateTimeStamp(), payloadLength};
        }

        static QByteArray combineToPacket(const PacketHeader& header, const Payload& payload)
        {
            QByteArray packet;
            QDataStream stream(&packet, QIODevice::WriteOnly);
            stream.writeRawData(reinterpret_cast<const char*>(&header), sizeof(PacketHeader));
            stream << payload.payload;

            return packet;
        }

        static QByteArray combineToPacket(const PacketHeader& header, const AuthorizationPayload& payload)
        {
            QByteArray packet;
            QDataStream stream(&packet, QIODevice::WriteOnly);
            stream.writeRawData(reinterpret_cast<const char*>(&header), sizeof(PacketHeader));
            stream << payload.username;
            stream << payload.password;

            return packet;
        }

    private:
        static QString generateTimeStamp()
        {
            return QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
        }
    };

}

#endif //SERVER_SHARED_H
