#include "Serializer.h"
#include <QDataStream>

#include <memory>

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
    PacketHeader header;
    Payload payload;
    QDataStream stream(&data, QIODevice::ReadOnly);
    stream.readRawData(reinterpret_cast<char*>(&header), sizeof(PacketHeader));
    stream >> payload.payload;

    return payload;
}

FilePaylaod Serializer::toFilePayload(QByteArray& data)
{
    PacketHeader header;
    FilePaylaod payload;
    QDataStream stream(&data, QIODevice::ReadOnly);
    stream.readRawData(reinterpret_cast<char*>(&header), sizeof(PacketHeader));
    stream >> payload.clientUuid;
    stream >> payload.fileName;
    stream >> payload.fileData;

    return payload;
}

std::pair<PacketHeader, AuthorizationPayload> Serializer::toAuthorizationPayload(QByteArray& data)
{
    PacketHeader header;
    AuthorizationPayload payload;
    QDataStream stream(&data, QIODevice::ReadOnly);
    stream.readRawData(reinterpret_cast<char*>(&header), sizeof(PacketHeader));
    stream >> payload.username;
    stream >> payload.password;

    return std::make_pair(header, payload);
}
