#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <memory>
#include <map>

#include "DatabaseController.h"

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QObject* parent = nullptr);

    virtual ~Server();

signals:

public slots:
    void handleNewConnection();


public:

private:
    QTcpServer* m_server;
    DatabaseController* m_dbController;
};


#endif //SERVER_SERVER_H
