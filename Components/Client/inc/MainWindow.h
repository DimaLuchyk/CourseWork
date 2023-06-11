#ifndef COURSEWORK_MAINWINDOW_H
#define COURSEWORK_MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include <memory>

#include "Client.h"
#include "NetworkClient.h"

namespace coursework::windows
{
    class MainWindow : public QWidget
    {
        Q_OBJECT
    public:
        MainWindow(std::shared_ptr<NetworkClient> client, QWidget* parent = nullptr);
        ~MainWindow();

    private slots:


    private:
        QLabel* m_line;
        QPushButton* m_btn;

        std::shared_ptr<NetworkClient> m_client;
    };
}

#endif //COURSEWORK_MAINWINDOW_H
