#ifndef COURSEWORK_PACKET_H
#define COURSEWORK_PACKET_H

#include <cstddef>
#include <cstdint>

#include <QString>
#include <QByteArray>

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

    struct FilePaylaod
    {
        QString clientUuid;
        QString fileName;
        QByteArray fileData;
    };

    struct Payload
    {
        QString payload;
    };

}

#endif //COURSEWORK_PACKET_H
