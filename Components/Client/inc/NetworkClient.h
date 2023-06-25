#ifndef SERVER_NETWORK_CLIENT_H
#define SERVER_NETWORK_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QUuid>
#include <QFile>
#include <QMessageBox>

#include "Packet.h"

namespace coursework
{
    class NetworkClient : public QObject
    {
        Q_OBJECT
    public:
        NetworkClient(QObject* parent = nullptr);

        ~NetworkClient();

        QTcpSocket* get();

        const QUuid& getId();

        void setId(const QUuid& id);

        bool connectToServer(const QString& ip, const std::uint16_t port);

        void sendData(const QByteArray &data);

    public slots:
        QByteArray readTcpData();

    signals:
        void loggedIn();
        void loggedUp(const QString& status);
        void failedToLogIn(const QString& status);
        void failedToLogUp(const QString& status);
        void receievedExistedFiles(const QString& files);
        void fileDownloaded(QByteArray fileData);

    private:
        QUuid m_uuid;
        QTcpSocket* m_socket;
        QString m_ip;
        std::uint16_t m_port = 0;
    };
}

#endif //SERVER_NETWORK_CLIENT_H