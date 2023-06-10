//
// Created by dany on 14.05.23.
//

#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

namespace coursework
{

    class Client : public QObject
    {
    public:
        //Q_OBJECT

    public:
        Client(QObject *parent = nullptr);

        ~Client();

    public:
        QTcpSocket* get();

        bool connect(const QString& ip, const std::uint16_t port);
        void sendData(const QByteArray &data) const;

    public slots:
        void readTcpData();

    private:
        QTcpSocket *m_socket;
        QString m_ip;
        std::uint16_t m_port = 0;
    };

}

#endif //SERVER_CLIENT_H
