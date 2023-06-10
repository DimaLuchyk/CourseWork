#include "Server.h"
#include "Shared.h"

#include <plog/Log.h>

#include <QThread>
#include <memory>

class ClientHandler : public QThread
{
public:
    ClientHandler(qintptr socketDescriptor, QObject* parent = nullptr)
    :
    QThread(parent),
    m_socketDescriptor{socketDescriptor},
    m_packetProcessor{nullptr}
    {
        PLOG_DEBUG << "ClientHandler ctor";
    }

    ~ClientHandler()
    {
        PLOG_DEBUG << "ClientHandler dtor";
    }

    void run() override
    {
        m_packetProcessor = std::make_unique<coursework::protocol::PacketProcessor>();
        m_socket = new QTcpSocket();

        if(!m_socket->setSocketDescriptor(m_socketDescriptor))
        {
            PLOG_WARNING << "failed to set descriptor to the socket. File descriptor: " << m_socketDescriptor;
            //emit error(m_socket->error());
            return;
        }

        connect(m_socket, &QTcpSocket::readyRead, this, &ClientHandler::handleClientPacket, Qt::DirectConnection);
        connect(m_socket, &QTcpSocket::disconnected, this, &ClientHandler::disconnected);

        PLOG_INFO << m_socketDescriptor << ": socket descriptor, connected to the server.";


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
    std::unique_ptr<coursework::protocol::PacketProcessor> m_packetProcessor;

    QTcpSocket* m_socket;
    qintptr m_socketDescriptor;
};

coursework::Server::Server(QObject* parent)
    :
    QTcpServer(parent)
{
    PLOG_DEBUG << "Server ctor";
}

coursework::Server::~Server()
{
    PLOG_DEBUG << "Server dtor";

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
    PLOG_INFO << "Server about to start at:" << serverAddress().toString() << ":" << serverPort();;

    if(!listen(QHostAddress::Any, 9999))
    {
        PLOG_ERROR << "Server failed to start";
        return false;
    }

    PLOG_INFO << "Server started";
    return true;
}

void coursework::Server::incomingConnection(qintptr socketDescriptor)
{
    PLOG_INFO << "new incomingConnection. SocketDescriptor: " << socketDescriptor;

    // Every new connection will be run in a newly created thread
    auto clientHandler = new ClientHandler(socketDescriptor, this);

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(clientHandler, &QThread::finished, clientHandler, &QThread::deleteLater);
    clientHandler->start();
}
