#ifndef COURSEWORK_MAINWINDOW_H
#define COURSEWORK_MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

namespace coursework::windows
{
    class MainWindow : public QWidget
    {
        Q_OBJECT
    public:
        MainWindow(QWidget* parent = nullptr){};
        ~MainWindow(){};

    private slots:


    private:
        QLabel* m_line;
        QPushButton* m_btn;
    };
}

#endif //COURSEWORK_MAINWINDOW_H
