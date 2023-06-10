#ifndef CLIENT_AUTHORISATIONWINDOW_H
#define CLIENT_AUTHORISATIONWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <memory>

#include "Client.h"

namespace coursework::windows
{
    class AuthorizationWindow : public QWidget
    {
        Q_OBJECT
    public:
        AuthorizationWindow(std::shared_ptr<Client> client, QWidget* parent = nullptr);
        ~AuthorizationWindow();

    private slots:
        void onLogInClicked();
        void onLogUpClicked();
        void handleData();

    private:
        QLineEdit* m_userNameTextLine;
        QLineEdit* m_passwordTextLine;
        QLabel* m_statusLine;
        QPushButton* m_loginButton;
        QPushButton* m_logupButton;

        std::shared_ptr<Client> m_client;
    };
}

#endif //CLIENT_AUTHORISATIONWINDOW_H
