#ifndef _PACKET_PROCESSOR_H_
#define _PACKET_PROCESSOR_H_

#include <QObject>
#include <QByteArray>
#include <QThreadPool>

namespace coursework::protocol
{

    class PacketProcessor : public QObject
    {
        Q_OBJECT
    public:
        PacketProcessor();
        ~PacketProcessor();

        void handlePacket(QByteArray packet);

    private:
        QThreadPool* m_theadPool;

    };

};

#endif _PACKET_PROCESSOR_H_