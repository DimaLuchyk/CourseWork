//
// Created by dany on 14.05.23.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <memory>
#include <map>

class Server : public QObject
{
public:
    Q_OBJECT

public:
    Server(QObject* parent = nullptr);

    ~Server();

signals:

public slots:
    void handleNewConnection();


public:

private:
    QTcpServer* m_server;
};


#endif //SERVER_SERVER_H
