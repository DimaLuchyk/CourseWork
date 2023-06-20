#include "PacketGenerator.h"
#include <QDataStream>
#include <QDateTime>

using namespace coursework::protocol;


PacketHeader PacketGenerator::generatePacketHeader(const PacketType packetType, const std::uint32_t payloadLength)
{
    return {STARTOFPACKET, 0, packetType, generateTimeStamp(), payloadLength};
}

QString PacketGenerator::generateTimeStamp()
{
    return QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
}