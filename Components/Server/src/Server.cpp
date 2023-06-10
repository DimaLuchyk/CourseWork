#include "Server.h"
#include "Shared.h"

#include <QThread>

class ClientHandler : public QThread
{
public:
    ClientHandler(qintptr socketDescriptor, QObject* parent = nullptr)
    :
    QThread(parent),
    m_socketDescriptor{socketDescriptor},
    m_packetProcessor{nullptr}
    {

    }

    void run() override
    {
        // thread starts here
        qDebug() << " Thread started";

        m_packetProcessor = new coursework::protocol::PacketProcessor(this);

        m_socket = new QTcpSocket();

        // set the ID
        if(!m_socket->setSocketDescriptor(m_socketDescriptor))
        {
            // something's wrong, we just emit a signal
            //emit error(m_socket->error());
            return;
        }

        // connect socket and signal
        // note - Qt::DirectConnection is used because it's multithreaded
        //        This makes the slot to be invoked immediately, when the signal is emitted.

        connect(m_socket, &QTcpSocket::readyRead, this, &ClientHandler::handleClientPacket, Qt::DirectConnection);
        connect(m_socket, &QTcpSocket::disconnected, this, &ClientHandler::disconnected);

        // We'll have multiple clients, we want to know which is which
        qDebug() << m_socketDescriptor << " Client connected";

        // make this thread a loop,
        // thread will stay alive so that signal/slot to function properly
        // not dropped out in the middle when thread dies

        exec();
    }

public slots:
    void handleClientPacket()
    {
        QByteArray packet = m_socket->readAll();
        m_socket->write(m_packetProcessor->handlePacket(packet));
    }

    void disconnected()
    {
        qDebug() << m_socketDescriptor << " Disconnected";


        m_socket->deleteLater();
    }

private:
    coursework::protocol::PacketProcessor *m_packetProcessor;

    QTcpSocket* m_socket;
    qintptr m_socketDescriptor;
};

coursework::Server::Server(QObject* parent)
    :
    QTcpServer(parent)
{

}

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

bool coursework::Server::start()
{
    if(!listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start";
        return false;
    }
    qDebug() << "Server started!";
    return true;
}

/*void coursework::Server::handleNewConnection()
{
    qDebug() << "handleNewConnection\n";

    QTcpSocket* client = m_server->nextPendingConnection();

    auto clientConnection = new Task(client);
    client->moveToThread(clientConnection);
    clientConnection->start();

    *//*QUuid uuid = QUuid::createUuid();
    m_clients.emplace(uuid, client);

    connect(client, &QTcpSocket::readyRead, this, &Server::handleClientPacket);*//*
}*/

/*void coursework::Server::handleClientPacket()
{
    qDebug() << "handleClientPacket\n";
    QTcpSocket* client = reinterpret_cast<QTcpSocket*>(sender());

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

}*/

// This function is called by QTcpServer when a new connection is available.
void coursework::Server::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    qDebug() << socketDescriptor << " Connecting...";

    // Every new connection will be run in a newly created thread
    auto clientHandler = new ClientHandler(socketDescriptor, this);

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(clientHandler, &QThread::finished, clientHandler, &QThread::deleteLater);
    clientHandler->start();
}
