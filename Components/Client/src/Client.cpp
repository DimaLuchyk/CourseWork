//
// Created by dany on 14.05.23.
//

#include "../inc/Client.h"

Client::Client(const QString& ip, const std::uint16_t port, QObject* parent)
    :
    QObject(parent),
    m_ip(ip),
    m_port(port)
{
    std::string str("hello from client");
    QByteArray data = QByteArray::fromStdString(str);

    m_socket = new QTcpSocket( this );
    connect( m_socket, &QTcpSocket::readyRead, this, &Client::readTcpData );

    m_socket->connectToHost(m_ip, m_port);
    if( m_socket->waitForConnected() )
    {
        qDebug() << "connected to host\n";
        qDebug() << "writing message to the server\n";
        m_socket->write( data );
    }
}

void Client::readTcpData()
{
    QByteArray data = m_socket->readAll();
    qDebug() << "from server: " << data;
}

Client::~Client()
{

}
