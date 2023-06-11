#include "PacketProcessor/Tasks.h"
#include "Shared.h"
#include "plog/Log.h"

#include <QUuid>

coursework::protocol::ITask::ITask(DatabaseController *dbController)
    :
    m_dbController(dbController)
{

}

coursework::protocol::LogUpTask::LogUpTask(const QString& userName, const QString& password,
                                           DatabaseController *dbController)
    :
    ITask(dbController),
    m_userName{userName},
    m_password{password}
{
    PLOG_DEBUG << "ctor";
}

coursework::protocol::LogUpTask::~LogUpTask() noexcept
{
    PLOG_DEBUG << "dtor";
}

QByteArray coursework::protocol::LogUpTask::perform()
{
    PLOG_DEBUG << "perform";

    if(!m_dbController->userExist(m_userName).isNull())
    {
        Payload payload{"The user is already registered"};
        PacketHeader header = PacketGenerator::generatePacketHeader(PacketType::LOG_UP_FAILURE, sizeof(payload));

        return PacketGenerator::combineToPacket(header, payload);
    }

    QUuid uuid = QUuid::createUuid();
    m_dbController->addUser(uuid, m_userName, m_password);

    Payload payload{"User registered"};
    PacketHeader header = PacketGenerator::generatePacketHeader(PacketType::LOG_UP_SUCCESS, sizeof(payload));

    return PacketGenerator::combineToPacket(header, payload);
}

coursework::protocol::LogInTask::LogInTask(const QString &userName, const QString &password, DatabaseController *dbController)
    :
    ITask(dbController),
    m_userName(userName),
    m_password(password)
{
    PLOG_DEBUG << "ctor";
}

coursework::protocol::LogInTask::~LogInTask()
{
    PLOG_DEBUG << "dtor";
}


QByteArray coursework::protocol::LogInTask::perform()
{
    PLOG_DEBUG << "perform";

    if(!m_dbController->userExist(m_userName).isNull())
    {
        Payload payload{"The user was found"};
        PacketHeader header = PacketGenerator::generatePacketHeader(PacketType::LOG_IN_SUCCESS, sizeof(payload));

        return PacketGenerator::combineToPacket(header, payload);
    }

    Payload payload{"User was not found. You should register first"};
    PacketHeader header = PacketGenerator::generatePacketHeader(PacketType::LOG_IN_FAILURE, sizeof(payload));

    return PacketGenerator::combineToPacket(header, payload);
}

