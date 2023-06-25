#include "NetworkClient.h"
#include "Serializer.h"

coursework::NetworkClient::NetworkClient(QObject* parent)
:
QObject(parent)
{}

coursework::NetworkClient::~NetworkClient()
{
    if(m_socket->isOpen())
    {
        m_socket->close();
    }
}

QTcpSocket* coursework::NetworkClient::get()
{
    return m_socket;
}

const QUuid& coursework::NetworkClient::getId()
{
    return m_uuid;
}

void coursework::NetworkClient::setId(const QUuid& id)
{
    m_uuid = id;
}

bool coursework::NetworkClient::connectToServer(const QString& ip, const std::uint16_t port)
{
    m_port = port;
    m_ip = ip;

    m_socket = new QTcpSocket(this);
    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &NetworkClient::readTcpData);

    m_socket->connectToHost(m_ip, m_port);
    if(m_socket->waitForConnected())
    {
        qDebug() << "connected\n";
        return true;
    }
    return false;
}

void coursework::NetworkClient::sendData(const QByteArray &data)
{
    if(m_socket->isOpen())
    {
        qDebug() << "wrote: " << m_socket->write(data);
        return;
    }
    else
    {
        qDebug() << "socket is not open!\n";
    }
}


QByteArray coursework::NetworkClient::readTcpData()
{
    if(m_socket->isOpen())
    {
        auto packet = m_socket->readAll();
        protocol::PacketHeader *header = reinterpret_cast<protocol::PacketHeader *>(packet.data());

        if (header->packetType == protocol::PacketType::LOG_IN_SUCCESS)
        {
            protocol::Payload payload;
            protocol::Serializer::toPayload(packet, *header, payload);

            setId(QUuid(payload.payload));

            emit loggedIn();
        }
        else if (header->packetType == protocol::PacketType::LOG_IN_FAILURE)
        {
            protocol::Payload payload;
            protocol::Serializer::toPayload(packet, *header, payload);

            emit failedToLogIn(payload.payload);
        }
        else if (header->packetType == protocol::PacketType::LOG_UP_SUCCESS)
        {
            protocol::Payload payload;
            protocol::Serializer::toPayload(packet, *header, payload);

            emit loggedUp(payload.payload);
        }
        else if (header->packetType == protocol::PacketType::LOG_UP_FAILURE)
        {
            protocol::Payload payload;
            protocol::Serializer::toPayload(packet, *header, payload);

            emit failedToLogUp(payload.payload);
        }
        else if (header->packetType == protocol::PacketType::GET_EXISTED_FILES_FAILURE)
        {
            QMessageBox::critical(nullptr, "Error", "Failed to receive existed files!");
        }
        else if (header->packetType == protocol::PacketType::GET_EXISTED_FILES_SUCCESS)
        {
            protocol::Payload payload;
            protocol::Serializer::toPayload(packet, *header, payload);

            emit receievedExistedFiles(payload.payload);
        }
        else if (header->packetType == protocol::PacketType::ADD_FILE_FAILURE)
        {
            QMessageBox::critical(nullptr, "Error", "Failed to add file!");
        }
        else if (header->packetType == protocol::PacketType::ADD_FILE_SUCCESS)
        {
            QMessageBox::information(nullptr, "Info", "File was added successfully!");
        }
        else if (header->packetType == protocol::PacketType::DOWNLOAD_FILE_SUCCESS)
        {
            protocol::FilePaylaod payload;
            protocol::Serializer::toFilePayload(packet, *header, payload);

            emit fileDownloaded(payload.fileData);
        }
        else if (header->packetType == protocol::PacketType::DOWNLOAD_FILE_FAILURE)
        {
            QMessageBox::critical(nullptr, "Error", "Failed to download file!");
        }
        else if (header->packetType == protocol::PacketType::REMOVE_FILE_SUCCESS)
        {
            QMessageBox::information(nullptr, "Info", "File was removed successfully!");
        }
        else if (header->packetType == protocol::PacketType::REMOVE_FILE_FAILURE)
        {
            QMessageBox::critical(nullptr, "Error", "Failed to remove file!");
        }
        return packet;
    }
}