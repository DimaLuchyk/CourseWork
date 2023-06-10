#include "PacketProcessor/Tasks.h"
#include "Shared.h"

#include <QUuid>

coursework::protocol::BaseTask::BaseTask(QTcpSocket* client, DatabaseController *dbController)
    :
    m_client{client},
    m_dbController(dbController)
{

}

coursework::protocol::LogUpTask::LogUpTask(QTcpSocket* client, const QString userName, const QString password,
                                           DatabaseController *dbController)
    :
    BaseTask(client, dbController),
    m_userName{userName},
    m_password{password}
{
    qDebug() << "LogUpTask ctor\n";
}

void coursework::protocol::LogUpTask::run()
{
    if(!m_dbController->userExist(m_userName).isNull())
    {
        Payload payload{"The user is already registered"};
        PacketHeader header = PacketGenerator::generatePacketHeader(PacketType::LOG_UP_FAILURE, sizeof(payload));

        m_client->write(PacketGenerator::combineToPacket(header, payload));

        qDebug() << "user already exists\n";

        return;
    }

    QUuid uuid = QUuid::createUuid();
    m_dbController->addUser(uuid, m_userName, m_password);

    Payload payload{"User registered"};
    PacketHeader header = PacketGenerator::generatePacketHeader(PacketType::LOG_UP_SUCCESS, sizeof(payload));

    m_client->write(PacketGenerator::combineToPacket(header, payload));


    qDebug() << "user registered\n";
}

void coursework::protocol::LogInTask::run()
{

}
