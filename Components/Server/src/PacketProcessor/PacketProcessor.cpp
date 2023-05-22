#include "PacketProcessor/PacketProcessor.h"
#include "PacketProcessor/Tasks.h"
#include "Shared.h"

#include <QDebug>

coursework::protocol::PacketProcessor::PacketProcessor()
{
    m_dbController = new DatabaseController("localhost", 5432, "mydatabase", "myuser", "mypassword");
    m_dbController->start();
}

coursework::protocol::PacketProcessor::~PacketProcessor()
{
    m_dbController->stop();
}

void coursework::protocol::PacketProcessor::handlePacket(QByteArray packet, QTcpSocket* client)
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
            auto payload = reinterpret_cast<AuthenticationPayload*>(packet.data() + sizeof(PacketHeader));
            auto task = new LogUpTask(client, payload->username, payload->password, m_dbController);
            m_theadPool->start(task);
            break;
        //case...
    }


}