//
// Created by dany on 14.05.23.
//

#include "../inc/Server.h"

Server::Server(QObject* parent)
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

Server::~Server()
{

}

void Server::handleNewConnection()
{
    qDebug() << "handleNewConnection\n";

    // need to grab the socket
    QTcpSocket *socket = m_server->nextPendingConnection();

    socket->write("Hello client\n");
    socket->flush();

    socket->waitForBytesWritten(3000);

    socket->close();
}