#ifndef CLIENT_AUTHORISATIONWINDOW_H
#define CLIENT_AUTHORISATIONWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

namespace coursework::windows
{
    class AuthorisationWindow : public QWidget
    {
        Q_OBJECT
    public:
        AuthorisationWindow();
        ~AuthorisationWindow();

    private slots:
        void onLogInClicked();
        void onLogUpClicked();

    private:
        QLineEdit* m_userNameTextLine;
        QLineEdit* m_passwordTextLine;
        QLabel* m_statusLine;
        QPushButton* m_loginButton;
        QPushButton* m_logupButton;
    };
}

#endif //CLIENT_AUTHORISATIONWINDOW_H
