#include "../inc/Client.h"
#include "../../Server/inc/Shared.h"


coursework::Client::Client(QObject *parent)
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

bool coursework::Client::connectToServer(const QString& ip, const std::uint16_t port)
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

void coursework::Client::sendData(const QByteArray &data) const
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


QByteArray coursework::Client::readTcpData()
{
    if(m_socket->isOpen())
    {
        auto packet = m_socket->readAll();
        return packet;
    }
}