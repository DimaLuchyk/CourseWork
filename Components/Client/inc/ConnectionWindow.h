#ifndef COURSEWORK_CONNECTWINDOW_H
#define COURSEWORK_CONNECTWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <memory>

#include "Client.h"

namespace coursework::windows
{
    class ConnectionWindow : public QWidget
    {
        Q_OBJECT
    public:
        ConnectionWindow(std::shared_ptr<Client> client, QWidget* parent = nullptr);
        ~ConnectionWindow();

    private slots:
        void connectToServer();

    signals:
        void connectedToServer();

    private:
        QLabel* m_ipLabel;
        QLabel* m_portLabel;
        QLineEdit* m_ipLineEdit;
        QLineEdit* m_portLineEdit;
        QLabel* m_statusBar;
        QPushButton* m_connectionButton;

        std::shared_ptr<Client> m_client;
    };
}

#endif //COURSEWORK_CONNECTWINDOW_H
