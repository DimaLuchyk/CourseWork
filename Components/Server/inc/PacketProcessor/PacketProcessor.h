#ifndef _PACKET_PROCESSOR_H_
#define _PACKET_PROCESSOR_H_

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QThreadPool>
#include <QRunnable>

#include "DatabaseController.h"

namespace coursework::protocol
{

    class PacketProcessor : public QObject
    {
        Q_OBJECT
    public:
        PacketProcessor(QObject* parent = nullptr);
        ~PacketProcessor();

        QByteArray handlePacket(QByteArray& packet);

    private:
        DatabaseController *m_dbController;
    };

};

#endif