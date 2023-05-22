#ifndef _PACKET_PROCESSOR_H_
#define _PACKET_PROCESSOR_H_

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QThreadPool>

#include "DatabaseController.h"

namespace coursework::protocol
{

    class PacketProcessor : public QObject
    {
        Q_OBJECT
    public:
        PacketProcessor();
        ~PacketProcessor();

        void handlePacket(QByteArray packet, QTcpSocket* client);

    private:
        DatabaseController *m_dbController;
        QThreadPool* m_theadPool;

    };

};

#endif