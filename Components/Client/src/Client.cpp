#include "../inc/Client.h"
#include "../../Server/inc/Shared.h"

coursework::Client::Client(QObject* parent)
    :
    QObject(parent)
{
}

coursework::Client::~Client()
{
    if(m_socket->isOpen())
    {
        m_socket->close();
    }
}

QTcpSocket* coursework::Client::get()
{
    return m_socket;
}

bool coursework::Client::connect(const QString& ip, const std::uint16_t port)
{
    m_port = port;
    m_ip = ip;

    m_socket = new QTcpSocket(this);
    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &Client::readTcpData);
    m_socket->connectToHost(m_ip, m_port);
    if(m_socket->waitForConnected())
    {
        qDebug() << "connected\n";
        return true;
    }
    return false;
}

void coursework::Client::sendData(const QByteArray& data) const
{
    if(m_socket->isOpen())
    {
        m_socket->write(data);
        return;
    }
    else
    {
        qDebug() << "socket is not open!\n";
    }
}

void coursework::Client::readTcpData()
{
    if(m_socket->isOpen())
    {
        auto packet = m_socket->readAll();
        coursework::protocol::Payload payload;
        coursework::protocol::PacketHeader header;

        QDataStream stream(&packet, QIODevice::ReadOnly);
        stream.readRawData(reinterpret_cast<char*>(&header), sizeof(coursework::protocol::PacketHeader));
        stream >> payload.payload;
        qDebug() << "from server: " << QString(payload.payload) << "\n";
        return;
    }

    qDebug() << "socket is not open!\n";
}
