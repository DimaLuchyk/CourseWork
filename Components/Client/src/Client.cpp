#include "../inc/Client.h"

coursework::client::Client::Client(const QString& ip, const std::uint16_t port, QObject* parent)
    :
    QObject(parent),
    m_ip(ip),
    m_port(port)
{
    m_socket = new QTcpSocket( this );
    connect( m_socket, &QTcpSocket::readyRead, this, &Client::readTcpData );

    m_socket->connectToHost(m_ip, m_port);
    if(m_socket->waitForConnected() )
    {
        qDebug() << "connected to host\n";
    }
}

void coursework::client::Client::sendData(const QByteArray& data)
{
    if(m_socket->isOpen())
    {
        m_socket->write(data);
    }
    else
    {
        qDebug() << "socket is not open!\n";
    }
}

void coursework::client::Client::readTcpData()
{

}

coursework::client::Client::~Client()
{
    if(m_socket->isOpen())
    {
        m_socket->close();
    }
}
