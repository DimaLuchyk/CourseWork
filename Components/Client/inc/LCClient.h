//
// Created by dany on 14.05.23.
//

#ifndef CLIENT_LCCLIENT_H
#define CLIENT_LCCLIENT_H

#include <QWidget>

#include "Client.h"

class LCClient : public QWidget
{
public:
    Q_OBJECT
public:
    LCClient(QObject* parent = nullptr);
    ~LCClient();

public slots:

signals:

private:
    Client* m_client;

};


#endif //CLIENT_LCCLIENT_H
