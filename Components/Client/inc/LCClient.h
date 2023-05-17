//
// Created by dany on 14.05.23.
//

#ifndef CLIENT_LCCLIENT_H
#define CLIENT_LCCLIENT_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#include "Client.h"

class LCClient : public QWidget
{
public:
    Q_OBJECT
public:
    LCClient(QObject* parent = nullptr);
    ~LCClient();

public slots:
    void connectToServer();
signals:

private:
    QLineEdit* m_ipLine;
    QLineEdit* m_portLine;
    QPushButton* m_connectionButton;

    Client* m_client;

};


#endif //CLIENT_LCCLIENT_H
