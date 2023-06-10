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

    class Server : public QObject
    {
        //Q_OBJECT
    public:
        Server(QObject *parent = nullptr);

        virtual ~Server();

    signals:

    public slots:

        void handleNewConnection();

        void handleClientPacket();


    public:



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
        QTcpServer *m_server;
        QTcpSocket* m_client;

        protocol::PacketProcessor *m_packetProcessor;

        std::unordered_map<QUuid, QTcpSocket *, QUuidHash> m_clients;
    };

}

#endif //SERVER_SERVER_H
