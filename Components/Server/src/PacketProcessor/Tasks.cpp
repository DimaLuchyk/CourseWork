#include "PacketProcessor/Tasks.h"
#include "Shared.h"

#include <QUuid>

coursework::protocol::ITask::ITask(DatabaseController *dbController)
    :
    m_dbController(dbController)
{

}

coursework::protocol::LogUpTask::LogUpTask(const QString userName, const QString password,
                                           DatabaseController *dbController)
    :
    ITask(dbController),
    m_userName{userName},
    m_password{password}
{
    qDebug() << "LogUpTask ctor\n";
}

QByteArray coursework::protocol::LogUpTask::perform()
{
    if(!m_dbController->userExist(m_userName).isNull())
    {
        Payload payload{"The user is already registered"};
        PacketHeader header = PacketGenerator::generatePacketHeader(PacketType::LOG_UP_FAILURE, sizeof(payload));

        //m_client->write(PacketGenerator::combineToPacket(header, payload));

        qDebug() << "user already exists\n";

        return PacketGenerator::combineToPacket(header, payload);
    }

    QUuid uuid = QUuid::createUuid();
    m_dbController->addUser(uuid, m_userName, m_password);

    Payload payload{"User registered"};
    PacketHeader header = PacketGenerator::generatePacketHeader(PacketType::LOG_UP_SUCCESS, sizeof(payload));

    qDebug() << "user registered\n";

    return PacketGenerator::combineToPacket(header, payload);
}

