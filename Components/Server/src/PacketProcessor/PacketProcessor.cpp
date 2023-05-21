#include "PacketProcessor/PacketProcessor.h"
#include "Shared.h"
#include <QDebug>

coursework::protocol::PacketProcessor::PacketProcessor()
{

}

coursework::protocol::PacketProcessor::~PacketProcessor()
{

}

void coursework::protocol::PacketProcessor::handlePacket(QByteArray packet)
{
    if(packet.isEmpty())
    {
        //log
        qDebug() << "packet is empty!";
        return;
    }

    PacketHeader* header = reinterpret_cast<PacketHeader*>(packet.data());

    switch(header->packetType)
    {
        case LOG_IN:
            break;
        case LOG_UP:
            break;
        //case...
    }


}