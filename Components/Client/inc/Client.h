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
    Q_OBJECT

public:
    Client(QObject* parent = nullptr);

    ~Client();

public slots:
    void readTcpData();
signals:

private:
    QTcpSocket* m_socket;
};


#endif //SERVER_CLIENT_H
