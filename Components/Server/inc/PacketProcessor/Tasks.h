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

    class TempTask : public ITask
    {
    public:
        TempTask(DatabaseController* dbController)
        :
        ITask(dbController)
        {

        }

        QByteArray perform() override
        {
            qDebug() << "TempTask";
            return {};
        }
    };

    class LogUpTask : public ITask
    {
    public:
        LogUpTask(const QString& userName, const QString& password, DatabaseController* dbController);
        ~LogUpTask();

        QByteArray perform() override;

    private:
        QString m_userName;
        QString m_password;
    };

    class LogInTask : public ITask
    {
    public:
        LogInTask(const QString& userName, const QString& password, DatabaseController* dbController);
        ~LogInTask();

        QByteArray perform() override;

    private:
        QString m_userName;
        QString m_password;

    };

    class GetExistedFilesTask : public ITask
    {
    public:
        GetExistedFilesTask(DatabaseController* dbController);
        ~GetExistedFilesTask();

        QByteArray perform() override;

    private:
    };

    class AddFileTask : public ITask
    {
    public:
        AddFileTask(const QString& fileName, const QByteArray& fileData, const QUuid& userId, DatabaseController* dbController);
        ~AddFileTask();

        QByteArray perform() override;

    private:
        QByteArray m_fileData;
        QString m_fileName;
        QUuid m_userId;
    };
}

#endif