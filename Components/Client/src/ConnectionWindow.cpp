#include "ConnectionWindow.h"

#include <QWidget>
#include <QGridLayout>

coursework::windows::ConnectionWindow::ConnectionWindow(std::shared_ptr<coursework::NetworkClient> client, QWidget *parent)
    :
    QWidget(parent),
    m_client(client)
{
    m_ipLabel = new QLabel("ip:", this);
    m_portLabel = new QLabel("port:", this);
    m_ipLineEdit = new QLineEdit(this);
    m_portLineEdit = new QLineEdit(this);
    m_statusBar = new QLabel(this);
    m_connectionButton = new QPushButton("Connect", this);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(m_ipLabel, 0, 0);
    layout->addWidget(m_portLabel, 0, 1);
    layout->addWidget(m_ipLineEdit, 1, 0);
    layout->addWidget(m_portLineEdit, 1, 1);
    layout->addWidget(m_connectionButton, 2, 0, 1, 2);
    setLayout(layout);

    connect(m_connectionButton, &QPushButton::clicked, this, &ConnectionWindow::connectToServer);
}

coursework::windows::ConnectionWindow::~ConnectionWindow()
{

}

void coursework::windows::ConnectionWindow::connectToServer()
{
    const std::uint16_t port = m_portLineEdit->text().toUInt();
    const QString ip = m_ipLineEdit->text();

    if(m_client->connectToServer(ip, port))
    {
        emit connectedToServer();
        qDebug() << "connected to server\n";
        return;
    }

    qDebug() << "failed to connect to the server\n";
    m_statusBar->setText("failed to connect");
}
