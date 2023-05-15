//
// Created by dany on 14.05.23.
//

#include "Server.h"

Server::Server(QObject* parent)
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

    m_dbController = new DatabaseController("localhost", 5432, "mydatabase", "myuser", "mypassword");
    m_dbController->start();

};

Server::~Server()
{
    m_dbController->stop();
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