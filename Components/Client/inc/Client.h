#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

//#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

//#include "../../Server/inc/Shared.h"

namespace coursework
{

    class Client : public QObject
    {
        //Q_OBJECT
    public:
        Client(QObject* parent = nullptr);

        ~Client();

        QTcpSocket* get();

        bool connectToServer(const QString& ip, const std::uint16_t port);

        void sendData(const QByteArray &data) const;

    public slots:
        QByteArray readTcpData();

    signals:
        void loggedIn();

    private:
        QTcpSocket* m_socket;
        QString m_ip;
        std::uint16_t m_port = 0;

    };

}

#endif //SERVER_CLIENT_H
