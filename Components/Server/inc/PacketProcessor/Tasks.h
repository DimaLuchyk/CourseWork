#ifndef _TASKS_H_
#define _TASKS_H_

#include <QRunnable>
#include <QTcpSocket>
#include <QVariant>
#include "DatabaseController.h"

namespace coursework::protocol
{
    //============================================================
    // BaseTask - base class for each task that server can process
    // does not implement run method inherited from QRunnable
    //=============================================================

    class BaseTask : public QRunnable
    {
    public:
        BaseTask(QTcpSocket* client, DatabaseController* dbController);

        //void run() override; - each specific task should override it

    protected:
        DatabaseController* m_dbController;
        QTcpSocket* m_client;
    };

    class LogUpTask : public BaseTask
    {
    public:
        LogUpTask(QTcpSocket* client, const QString userName, const QString password, DatabaseController* dbController);

        void run() override;

    private:
        QString m_userName;
        QString m_password;
    };

    class LogInTask : QRunnable
    {
    public:
        LogInTask(DatabaseController* dbController);

        void run() override;

    private:
        QString m_userName;
        QString m_password;

    };

    //other
}

#endif