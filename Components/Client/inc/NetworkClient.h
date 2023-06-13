#ifndef SERVER_NETWORK_CLIENT_H
#define SERVER_NETWORK_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QUuid>

#include "../../Server/inc/Shared.h"

namespace coursework
{
    class NetworkClient : public QObject
    {
        Q_OBJECT
    public:
        NetworkClient(QObject* parent = nullptr)
            :
        QObject(parent)
        {

        }

        ~NetworkClient()
        {
            if(m_socket->isOpen())
            {
                m_socket->close();
            }
        }

        QTcpSocket* get()
        {
            return m_socket;
        }

        const QUuid& getId()
        {
            return m_uuid;
        }

        void setId(const QUuid& id)
        {
            m_uuid = id;
        }

        bool connectToServer(const QString& ip, const std::uint16_t port)
        {
            m_port = port;
            m_ip = ip;

            m_socket = new QTcpSocket(this);
            QObject::connect(m_socket, &QTcpSocket::readyRead, this, &NetworkClient::readTcpData);

            m_socket->connectToHost(m_ip, m_port);
            if(m_socket->waitForConnected())
            {
                qDebug() << "connected\n";
                return true;
            }
            return false;
        }

        void sendData(const QByteArray &data) const
        {
            if(m_socket->isOpen())
            {
                qDebug() << "wrote: " << m_socket->write(data);
                return;
            }
            else
            {
                qDebug() << "socket is not open!\n";
            }
        }

    public slots:
        QByteArray readTcpData()
        {
            if(m_socket->isOpen())
            {
                auto packet = m_socket->readAll();
                coursework::protocol::PacketHeader* header = reinterpret_cast<coursework::protocol::PacketHeader*>(packet.data());

                if(header->packetType == coursework::protocol::PacketType::LOG_IN_SUCCESS)
                {
                    coursework::protocol::Payload payload;

                    QDataStream stream(&packet, QIODevice::ReadOnly);
                    stream.readRawData(reinterpret_cast<char *>(header), sizeof(coursework::protocol::PacketHeader));
                    stream >> payload.payload;

                    setId(QUuid(payload.payload));

                    emit loggedIn();
                }
                else if(header->packetType == coursework::protocol::PacketType::LOG_IN_FAILURE)
                {
                    coursework::protocol::Payload payload;

                    QDataStream stream(&packet, QIODevice::ReadOnly);
                    stream.readRawData(reinterpret_cast<char *>(header), sizeof(coursework::protocol::PacketHeader));
                    stream >> payload.payload;

                    emit failedToLogIn(payload.payload);
                }
                else if(header->packetType == coursework::protocol::PacketType::LOG_UP_SUCCESS)
                {
                    coursework::protocol::Payload payload;

                    QDataStream stream(&packet, QIODevice::ReadOnly);
                    stream.readRawData(reinterpret_cast<char *>(header), sizeof(coursework::protocol::PacketHeader));
                    stream >> payload.payload;

                    emit loggedUp(payload.payload);
                }
                else if(header->packetType == coursework::protocol::PacketType::LOG_UP_FAILURE)
                {
                    coursework::protocol::Payload payload;

                    QDataStream stream(&packet, QIODevice::ReadOnly);
                    stream.readRawData(reinterpret_cast<char *>(header), sizeof(coursework::protocol::PacketHeader));
                    stream >> payload.payload;

                    emit failedToLogUp(payload.payload);
                }
                else if(header->packetType == coursework::protocol::PacketType::GET_EXISTED_FILES_FAILURE)
                {
                    qDebug() << "GET_EXISTED_FILES_FAILURE";
                }
                else if(header->packetType == coursework::protocol::PacketType::GET_EXISTED_FILES_SUCCESS)
                {
                    coursework::protocol::Payload payload;

                    QDataStream stream(&packet, QIODevice::ReadOnly);
                    stream.readRawData(reinterpret_cast<char* >(header), sizeof(coursework::protocol::PacketHeader));
                    stream >> payload.payload;

                    emit receievedExistedFiles(payload.payload);
                }
                else if(header->packetType == coursework::protocol::PacketType::ADD_FILE_FAILURE)
                {
                    qDebug() << "ADD_FILE_FAILURE";
                }
                else if(header->packetType ==coursework::protocol::PacketType::ADD_FILE_SUCCESS)
                {
                    qDebug() << "ADD_FILE_SUCCESS";
                }

                return packet;
            }
        }

    signals:
        void loggedIn();
        void loggedUp(const QString& status);
        void failedToLogIn(const QString& status);
        void failedToLogUp(const QString& status);
        void receievedExistedFiles(const QString& files);

    private:
        QUuid m_uuid;
        QTcpSocket* m_socket;
        QString m_ip;
        std::uint16_t m_port = 0;
    };
}

#endif //SERVER_NETWORK_CLIENT_H