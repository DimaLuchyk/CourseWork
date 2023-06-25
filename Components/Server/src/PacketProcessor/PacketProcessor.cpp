#include "PacketProcessor/PacketProcessor.h"
#include "PacketProcessor/Tasks.h"
#include "Packet.h"
#include "Serializer.h"
#include "plog/Log.h"

#include <QDebug>

coursework::protocol::PacketProcessor::PacketProcessor(QObject* parent)
    :
    QObject(parent)
{
    PLOG_DEBUG << "PacketProcessor ctor";

    m_dbController = new DatabaseController("localhost", 5432, "mydatabase", "myuser", "mypassword", this);
    m_dbController->start();

}

coursework::protocol::PacketProcessor::~PacketProcessor()
{
    PLOG_DEBUG << "PacketProcessor dtor";
    m_dbController->stop();
}

QByteArray coursework::protocol::PacketProcessor::handlePacket(QByteArray& packet)
{
    if(packet.isEmpty())
    {
        PLOG_WARNING << "received empty packet for processing";
        return {};
    }

    PacketHeader* header = reinterpret_cast<PacketHeader*>(packet.data());

    std::shared_ptr<ITask> task;

    if(header->packetType == PacketType::LOG_IN)
    {
        AuthorizationPayload payload;
        Serializer::toAuthorizationPayload(packet, *header, payload);
        task = std::make_shared<LogInTask>(payload.username, payload.password, m_dbController);
    }
    else if(header->packetType == PacketType::LOG_UP)
    {
        AuthorizationPayload payload;
        Serializer::toAuthorizationPayload(packet, *header, payload);
        task = std::make_shared<LogUpTask>(payload.username, payload.password, m_dbController);
    }
    else if(header->packetType == PacketType::GET_EXISTED_FILES)
    {
        task = std::make_shared<GetExistedFilesTask>(m_dbController);
    }
    else if(header->packetType == PacketType::ADD_FILE)
    {
        FilePaylaod payload = Serializer::toFilePayload(packet);
        qDebug() << "fileName:" << payload.fileName;
        task = std::make_shared<AddFileTask>(payload.fileName, payload.fileData, QUuid(payload.clientUuid), m_dbController);
    }
    else if(header->packetType == PacketType::DOWNLOAD_FILE)
    {
        Payload payload = Serializer::toPayload(packet);
        task = std::make_shared<DownloadFileTask>(payload.payload, m_dbController);
    }
    else if(header->packetType == PacketType::REMOVE_FILE)
    {
        Payload payload = Serializer::toPayload(packet);
        task = std::make_shared<RemoveFileTask>(payload.payload, m_dbController);
    }
    else
    {
        PLOG_WARNING << "UNKNOWN PacketType received. PacketType: " << header->packetType;
    }

    return task->perform();
}
