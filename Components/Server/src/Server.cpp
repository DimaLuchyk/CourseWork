//
// Created by dany on 14.05.23.
//

#include "Server.h"

coursework::Server::Server(QObject* parent)
    :
    QObject(parent)
{
    m_server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(m_server, &QTcpServer::newConnection,
            this, &Server::handleNewConnection);

    if(!m_server->listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }

};

coursework::Server::~Server()
{
    //close connections with clients
    for(const auto& client : m_clients)
    {
        if(client.second->isOpen())
        {
            client.second->close();
        }
    }
}

void coursework::Server::handleNewConnection()
{
    qDebug() << "handleNewConnection\n";

    QTcpSocket *socket = m_server->nextPendingConnection();

    QUuid uuid = QUuid::createUuid();
    m_clients.emplace(uuid, socket);

    connect(socket, &QTcpSocket::readyRead, this, &Server::handleClientPacket);
}

void coursework::Server::handleClientPacket()
{
    qDebug() << "handleClientPacket\n";
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (client)
    {
        QByteArray packet = client->readAll();
        m_packetProcessor->handlePacket(packet, client);
    }
    else
    {
        // Failed to retrieve the sender as QTcpSocket
        // Handle the situation accordingly
    }

}
