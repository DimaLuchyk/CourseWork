#ifndef _TASKS_H_
#define _TASKS_H_

#include <QRunnable>
#include <QTcpSocket>
#include <QVariant>
#include <QByteArray>
#include <memory>
#include "DatabaseController.h"

namespace coursework::protocol
{
    //============================================================
    // BaseTask - base class for each task that server can process
    // does not implement run method inherited from QRunnable
    //=============================================================

    class ITask : QObject
    {

    public:
        ITask(DatabaseController* dbController);

        virtual QByteArray perform() = 0;

    protected:
        DatabaseController* m_dbController;
    };

    class LogUpTask : public ITask
    {
    public:
        LogUpTask(const QString userName, const QString password, DatabaseController* dbController);

        QByteArray perform() override;

    private:
        QString m_userName;
        QString m_password;
    };

    /*class LogInTask : public BaseTask
    {
    public:
        LogInTask(DatabaseController* dbController);

        void run() override;

    private:
        QString m_userName;
        QString m_password;

    };*/

    //other
}

#endif