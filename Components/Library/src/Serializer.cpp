#include "Serializer.h"
#include <QDataStream>

using namespace coursework::protocol;

QByteArray Serializer::combineToPacket(const PacketHeader& header, const Payload& payload)
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.writeRawData(reinterpret_cast<const char*>(&header), sizeof(PacketHeader));
    stream << payload.payload;

    return packet;
}

QByteArray Serializer::combineToPacket(const PacketHeader& header, const FilePaylaod& payload)
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.writeRawData(reinterpret_cast<const char*>(&header), sizeof(PacketHeader));
    stream << payload.clientUuid;
    stream << payload.fileName;
    stream << payload.fileData;

    return packet;
}

QByteArray Serializer::combineToPacket(const PacketHeader& header, const AuthorizationPayload& payload)
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.writeRawData(reinterpret_cast<const char*>(&header), sizeof(PacketHeader));
    stream << payload.username;
    stream << payload.password;

    return packet;
}

Payload Serializer::toPayload(QByteArray& data)
{
    Payload payload;
    QDataStream stream(&data, QIODevice::ReadOnly);
    stream.readRawData(reinterpret_cast<char*>(m_header), sizeof(PacketHeader));
    stream >> payload.payload;

    return payload;
}

FilePaylaod Serializer::toFilePayload(QByteArray& data)
{
    FilePaylaod payload;
    QDataStream stream(&data, QIODevice::ReadOnly);
    stream.readRawData(reinterpret_cast<char*>(m_header), sizeof(PacketHeader));
    stream >> payload.clientUuid;
    stream >> payload.fileName;
    stream >> payload.fileData;

    return payload;
}

AuthorizationPayload Serializer::toAuthorizationPayload(QByteArray& data)
{
    AuthorizationPayload payload;
    QDataStream stream(&data, QIODevice::ReadOnly);
    stream.readRawData(reinterpret_cast<char*>(m_header), sizeof(PacketHeader));
    stream >> payload.username;
    stream >> payload.password;

    return payload;
}
