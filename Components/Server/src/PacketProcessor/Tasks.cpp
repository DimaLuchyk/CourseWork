#include "PacketProcessor/Tasks.h"
#include "Shared.h"
#include "plog/Log.h"
#include "FileManager.h"

#include <QUuid>
#include <QFileInfo>

#define DEFAULT_PATH "/home/dany/server/files"

coursework::protocol::ITask::ITask(DatabaseController *dbController)
    :
    m_dbController(dbController)
{

}

QByteArray coursework::protocol::UnknownTask::perform()
{
    PLOG_DEBUG << "perform";

    return {};
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


    Payload payload;
    for (const auto& file : files)
    {
        payload.payload += file + "|"; // use '|' as a delimeter
    }

    auto header = PacketGenerator::generatePacketHeader(PacketType::GET_EXISTED_FILES_SUCCESS, sizeof(payload));

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

coursework::protocol::DownloadFileTask::DownloadFileTask(const QString& fileName, DatabaseController* dbController)
    :
    ITask(dbController),
    m_fileName(fileName)
{
    PLOG_DEBUG << "ctor";
}
coursework::protocol::DownloadFileTask::~DownloadFileTask()
{
    PLOG_DEBUG << "dtor";
}

QByteArray coursework::protocol::DownloadFileTask::perform()
{
    PLOG_DEBUG << "perform";

    coursework::protocol::PacketHeader header;
    coursework::protocol::FilePaylaod payload;

    do
    {
        if(!m_dbController->fileExist(m_fileName, DEFAULT_PATH))
        {
            PLOG_WARNING << "No file. Filename: " << m_fileName;
            break;
        }

        QFile file(QString(DEFAULT_PATH) + "/" + m_fileName);
        if(!file.open(QIODevice::ReadOnly))
        {
            PLOG_WARNING << "Failed to open file. Filename: " << m_fileName;
            break;
        }

        payload.clientUuid = QUuid().toString();
        QFileInfo fileInfo(m_fileName);
        payload.fileName = fileInfo.fileName();
        payload.fileData = file.readAll();
        auto header = coursework::protocol::PacketGenerator::generatePacketHeader(protocol::PacketType::DOWNLOAD_FILE_SUCCESS, sizeof(payload));

        return coursework::protocol::PacketGenerator::combineToPacket(header, payload);

    } while (false);


    header = coursework::protocol::PacketGenerator::generatePacketHeader(PacketType::DOWNLOAD_FILE_FAILURE, sizeof(payload));
    return coursework::protocol::PacketGenerator::combineToPacket(header, payload);
}

coursework::protocol::RemoveFileTask::RemoveFileTask(const QString& fileName, DatabaseController* dbContorller)
    :
    ITask(dbContorller),
    m_fileName(fileName)
{
    PLOG_DEBUG << "ctor";
}

coursework::protocol::RemoveFileTask::~RemoveFileTask()
{
    PLOG_DEBUG << "dtor";
}

QByteArray coursework::protocol::RemoveFileTask::perform()
{
    PLOG_DEBUG << "perform";

    coursework::protocol::PacketHeader header;
    coursework::protocol::FilePaylaod payload;

    do
    {
        if(!m_dbController->fileExist(m_fileName, DEFAULT_PATH))
        {
            PLOG_WARNING << "No file. Filename: " << m_fileName;
            break;
        }

        if(!m_dbController->removeFile(m_fileName, DEFAULT_PATH))
        {
            PLOG_WARNING << "Failed to remove file from db. FileName: " << m_fileName;
            break;
        }

        if(!coursework::managers::FileManager::deleteFile(m_fileName, DEFAULT_PATH))
        {
            PLOG_WARNING << "Filed to delete file from filesystem. FileName: " << m_fileName;
            break;
        }

        header = coursework::protocol::PacketGenerator::generatePacketHeader(protocol::PacketType::REMOVE_FILE_SUCCESS, sizeof(payload));
        return coursework::protocol::PacketGenerator::combineToPacket(header, payload);
    }
    while(false);

    header = coursework::protocol::PacketGenerator::generatePacketHeader(PacketType::REMOVE_FILE_FAILURE, sizeof(payload));
    return coursework::protocol::PacketGenerator::combineToPacket(header, payload);
}

