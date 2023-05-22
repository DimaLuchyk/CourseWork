//
// Created by dany on 14.05.23.
//

#include <QGridLayout>

#include "../inc/LCClient.h"

LCClient::LCClient(QObject* parent)
    :
    m_ipLine{new QLineEdit(this)},
    m_portLine{new QLineEdit(this)},
    m_connectionButton{new QPushButton("Connect", this)}
{

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(m_ipLine, 0, 0);
    layout->addWidget(m_portLine, 0, 1);
    layout->addWidget(m_connectionButton, 1, 0, 1, 2);
    setLayout(layout);

    connect(m_connectionButton, &QPushButton::clicked, this, &LCClient::connectToServer);
}

void LCClient::connectToServer()
{
    m_client = new Client(m_ipLine->text(), m_portLine->text().toUInt(), this);

}

LCClient::~LCClient()
{

}