#include "AuthorisationWindow.h"

#include <QLabel>
#include <QGridLayout>

coursework::windows::AuthorisationWindow::AuthorisationWindow()
    :
    m_userNameTextLine(new QLineEdit(this)),
    m_passwordTextLine(new QLineEdit(this)),
    m_statusLine(new QLabel("status line", this)),
    m_loginButton(new QPushButton("Log In", this)),
    m_logupButton(new QPushButton("Log Up", this))
{
    QLabel* userNameLabel = new QLabel("username", this);
    QLabel* passwordLabel = new QLabel("password", this);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(userNameLabel, 0, 0, 1, 1);
    layout->addWidget(m_userNameTextLine, 1, 0, 1, 2);
    layout->addWidget(passwordLabel, 2, 0, 1, 1);
    layout->addWidget(m_passwordTextLine, 3, 0, 1, 2);
    layout->addWidget(m_statusLine, 4, 0, 1, 2);
    layout->addWidget(m_loginButton, 4, 0, 1, 1);
    layout->addWidget(m_logupButton, 4, 1, 1, 1);
    setLayout(layout);
}

coursework::windows::AuthorisationWindow::~AuthorisationWindow() noexcept
{

}

void coursework::windows::AuthorisationWindow::onLogInClicked()
{

}
void coursework::windows::AuthorisationWindow::onLogUpClicked()
{

}
