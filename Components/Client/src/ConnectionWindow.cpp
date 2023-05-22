#include "ConnectionWindow.h"

#include <QGridLayout>

coursework::windows::ConnectionWindow::ConnectionWindow()
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
}

coursework::windows::ConnectionWindow::~ConnectionWindow() noexcept
{

}

void coursework::windows::ConnectionWindow::onConnectClicked()
{
    const QString ip = m_ipLine->text();
    const std::uint16_t port = m_portLine->text().toUInt();

    QTcpSocket* socket = new QTcpSocket( this );
    socket->connectToHost(ip, port);
    if(socket->waitForConnected() )
    {
        emit connectionSuccessful(socket);
    }
    else
    {
        //ToDo process
    }
}