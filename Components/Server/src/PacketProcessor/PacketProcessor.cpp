#include "PacketProcessor/PacketProcessor.h"
#include "PacketProcessor/Tasks.h"
#include "Shared.h"

#include <QDebug>

coursework::protocol::PacketProcessor::PacketProcessor(QObject* parent)
    :
    QObject(parent)
{
    m_dbController = new DatabaseController("localhost", 5432, "mydatabase", "myuser", "mypassword", this);
    m_dbController->start();

    QThreadPool::globalInstance()->setMaxThreadCount(8);

}

coursework::protocol::PacketProcessor::~PacketProcessor()
{
    m_dbController->stop();
}

void coursework::protocol::PacketProcessor::handlePacket(QByteArray& packet, QTcpSocket* client)
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
            AuthorizationPayload payload;

            QDataStream stream(&packet, QIODevice::ReadOnly);
            stream.readRawData(reinterpret_cast<char*>(header), sizeof(PacketHeader));
            stream >> payload.username;
            stream >> payload.password;

            QString name = payload.username;
            QString password = payload.password;

            auto task = new LogUpTask(client, payload.username, payload.password, m_dbController);

           /* QObject::connect(task, &LogUpTask::taskCompleted, this, [=](const QByteArray& data){
                client->write(data);
                client->flush();
            });*/

            QThreadPool::globalInstance()->start(task);
            //task->run();
            break;
        //case...
    }


}