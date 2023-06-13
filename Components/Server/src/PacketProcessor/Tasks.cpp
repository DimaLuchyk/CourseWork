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

    auto clientUuid = m_dbController->checkUserCredentials(m_userName, m_password);
    if(!clientUuid.isNull())
    {
        Payload payload{clientUuid.toString()};
        PacketHeader header = PacketGenerator::generatePacketHeader(PacketType::LOG_IN_SUCCESS, sizeof(payload));

        return PacketGenerator::combineToPacket(header, payload);
    }

    Payload payload{"You should register first. Or wrong credentials."};
    PacketHeader header = PacketGenerator::generatePacketHeader(PacketType::LOG_IN_FAILURE, sizeof(payload));

    return PacketGenerator::combineToPacket(header, payload);
}


coursework::protocol::GetExistedFilesTask::GetExistedFilesTask(DatabaseController* dbController)
    :
    ITask(dbController)
{
    PLOG_DEBUG << "ctor";
}

coursework::protocol::GetExistedFilesTask::~GetExistedFilesTask()
{
    PLOG_DEBUG << "dtor";
}

QByteArray coursework::protocol::GetExistedFilesTask::perform()
{
    PLOG_DEBUG << "perform";

    const auto files = m_dbController->getExistedFiles();
    if(!files.isEmpty())
    {
        Payload payload;
        for (const auto& file : files)
        {
            payload.payload += file + "|"; // use '|' as a delimeter
        }

        auto header = PacketGenerator::generatePacketHeader(PacketType::GET_EXISTED_FILES_SUCCESS, sizeof(payload));

        return PacketGenerator::combineToPacket(header, payload);
    }

    Payload payload{"failed to receive existed files"};
    auto header = PacketGenerator::generatePacketHeader(PacketType::GET_EXISTED_FILES_FAILURE, sizeof(payload));

    return PacketGenerator::combineToPacket(header, payload);
}

coursework::protocol::AddFileTask::AddFileTask(DatabaseController* dbController)
    :
    ITask(dbController)
{
    PLOG_DEBUG << "ctor";
}

coursework::protocol::AddFileTask::~AddFileTask()
{
    PLOG_DEBUG << "dtor";
}

QByteArray coursework::protocol::AddFileTask::perform()
{
    PLOG_DEBUG << "perform";
   // m_dbController->addFile()
}
