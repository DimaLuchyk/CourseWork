#ifndef CLIENT_CONNECTIONWINDOW_H
#define CLIENT_CONNECTIONWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>

namespace coursework::windows
{
    class ConnectionWindow : public QWidget
    {
        Q_OBJECT
    public:
        ConnectionWindow();
        ~ConnectionWindow();

    signals:
        void connectionSuccessful(QTcpSocket* socket);
    private slots:
        void onConnectClicked();

    private:
        QLineEdit* m_ipLine;
        QLineEdit* m_portLine;
        QPushButton* m_connectionButton;
    };
};

#endif //CLIENT_CONNECTIONWINDOW_H
