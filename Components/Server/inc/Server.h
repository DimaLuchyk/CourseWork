#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUuid>

#include <map>

#include "PacketProcessor/PacketProcessor.h"

namespace coursework
{

    class Server : public QTcpServer
    {
        //Q_OBJECT
    public:
        explicit Server(QObject *parent = nullptr);

        virtual ~Server();

    public:
        bool start();

    public slots:

        void handleNewConnection();

        void handleClientPacket();


    protected:
        void incomingConnection(qintptr socketDescriptor);

    private:
        class QUuidHash
        {
        public:
            std::size_t operator()(const QUuid &uuid) const
            {
                return qHash(uuid);
            }
        };

    private:
        QTcpSocket* m_client;

        std::unordered_map<QUuid, QTcpSocket *, QUuidHash> m_clients;
    };

}

#endif //SERVER_SERVER_H
