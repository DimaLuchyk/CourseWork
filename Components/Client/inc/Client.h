//
// Created by dany on 14.05.23.
//

#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
public:
    //Q_OBJECT

public:
    Client(const QString& ip, const std::uint16_t port, QObject* parent = nullptr);

    ~Client();

public slots:
    void readTcpData();
signals:

private:
    QTcpSocket* m_socket;
    QString m_ip;
    std::uint16_t m_port;
};


#endif //SERVER_CLIENT_H
