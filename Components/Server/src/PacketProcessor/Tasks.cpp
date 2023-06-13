#include "PacketProcessor/Tasks.h"
#include "Shared.h"
#include "plog/Log.h"
#include "FileManager.h"

#include <QUuid>

#define DEFAULT_PATH "/home/dany/server/files"

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

coursework::protocol::AddFileTask::AddFileTask(const QString& fileName, const QByteArray& fileData, const QUuid& userId, DatabaseController* dbController)
    :
    ITask(dbController),
    m_fileName(fileName),
    m_fileData(fileData),
    m_userId(userId)
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

    QUuid uuid = QUuid::createUuid();
    m_dbController->addFile(uuid, m_fileName, DEFAULT_PATH, m_userId.toString());

    if(managers::FileManager::createFile(m_fileName, DEFAULT_PATH, m_fileData))
    {
        PLOG_INFO << m_fileName  << " file, was successfully added to the server";

        Payload payload{m_fileName + "was added to the server"};
        auto header = coursework::protocol::PacketGenerator::generatePacketHeader(PacketType::ADD_FILE_SUCCESS, sizeof(payload));

        return coursework::protocol::PacketGenerator::combineToPacket(header, payload);
    }

    PLOG_WARNING << m_fileName << " file, was not added to the server";
    Payload payload{m_fileName + "failed to add to the server"};
    auto header = coursework::protocol::PacketGenerator::generatePacketHeader(PacketType::ADD_FILE_FAILURE, sizeof(payload));

    return coursework::protocol::PacketGenerator::combineToPacket(header, payload);
}
