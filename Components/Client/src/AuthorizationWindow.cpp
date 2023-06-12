#include "AuthorizationWindow.h"
#include "../../Server/inc/Shared.h"
#include "NetworkClient.h"

#include <QLabel>
#include <QGridLayout>

coursework::windows::AuthorizationWindow::AuthorizationWindow(std::shared_ptr<NetworkClient> client, QWidget* parent)
    :
    QWidget(parent),
    m_userNameTextLine(new QLineEdit(this)),
    m_passwordTextLine(new QLineEdit(this)),
    m_statusLine(new QLabel("status line", this)),
    m_loginButton(new QPushButton("Log In", this)),
    m_logupButton(new QPushButton("Log Up", this)),
    m_client(client)
{
    QLabel* userNameLabel = new QLabel("username", this);
    QLabel* passwordLabel = new QLabel("password", this);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(userNameLabel, 0, 0, 1, 1);
    layout->addWidget(m_userNameTextLine, 1, 0, 1, 2);
    layout->addWidget(passwordLabel, 2, 0, 1, 1);
    layout->addWidget(m_passwordTextLine, 3, 0, 1, 2);
    layout->addWidget(m_statusLine, 4, 0, 1, 2);
    layout->addWidget(m_loginButton, 5, 0, 1, 1);
    layout->addWidget(m_logupButton, 5, 1, 1, 1);
    setLayout(layout);

    connect(m_loginButton, &QPushButton::clicked, this, &AuthorizationWindow::onLogInClicked);
    connect(m_logupButton, &QPushButton::clicked, this, &AuthorizationWindow::onLogUpClicked);
    connect(m_client.get(), &coursework::NetworkClient::loggedUp, m_statusLine, &QLabel::setText);
    connect(m_client.get(), &coursework::NetworkClient::failedToLogUp, m_statusLine, &QLabel::setText);
    connect(m_client.get(), &coursework::NetworkClient::failedToLogIn, m_statusLine, &QLabel::setText);

}

coursework::windows::AuthorizationWindow::~AuthorizationWindow()
{

}

void coursework::windows::AuthorizationWindow::onLogInClicked()
{
    coursework::protocol::AuthorizationPayload payload;
    payload.username = m_userNameTextLine->text();
    payload.password = m_passwordTextLine->text();

    if(payload.username.isEmpty() || payload.password.isEmpty())
    {
        qDebug() << "not valid credentials";
        return;
    }

    auto header = coursework::protocol::PacketGenerator::generatePacketHeader(coursework::protocol::PacketType::LOG_IN,
                                                                              sizeof(payload));

    m_client->sendData(coursework::protocol::PacketGenerator::combineToPacket(header, payload));
}

void coursework::windows::AuthorizationWindow::onLogUpClicked()
{
    coursework::protocol::AuthorizationPayload payload;
    payload.username = m_userNameTextLine->text();
    payload.password = m_passwordTextLine->text();

    if(payload.username.isEmpty() || payload.password.isEmpty())
    {
        qDebug() << "not valid credentials";
        return;
    }

    auto header = coursework::protocol::PacketGenerator::generatePacketHeader(coursework::protocol::PacketType::LOG_UP,
                                                                              sizeof(payload));

    m_client->sendData(coursework::protocol::PacketGenerator::combineToPacket(header, payload));
}
