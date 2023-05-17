#include "DatabaseController.h"

#include <QDebug>

DatabaseController::DatabaseController(const QString& host, const std::uint16_t port, const QString& databaseName,
                                       const QString& userName, const QString& password, QObject* parent)
    :
    QObject(parent),
    m_host(host),
    m_port(port),
    m_databaseName(databaseName),
    m_userName(userName),
    m_password(password)
{
    m_database = QSqlDatabase::addDatabase("QPSQL");
    m_database.setHostName(m_host);
    m_database.setPort(m_port);
    m_database.setDatabaseName(m_databaseName);
    m_database.setUserName(m_userName);
    m_database.setPassword(m_password);
}

void DatabaseController::start()
{
    if (m_database.open())
    {
        qDebug() << "Connected!\n";
    }
    else
    {
        qDebug() << "Database Error:" << m_database.lastError().text() + "\n";
    }

}

void DatabaseController::stop()
{
    if(m_database.isOpen())
    {
        m_database.close();
        qDebug() << "database connection was closed!\n";
    }
}

void DatabaseController::addUser(const QUuid& uuid, const QString& userName, const QString& password)
{
    if(!m_database.isOpen())
    {
        qDebug() << "addUser failed to execute. database connection is not opened!\n";
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (user_id, user_name, password) VALUES (:userid :username, :password)");
    query.bindValue(":userid", uuid.toString());
    query.bindValue(":username", userName);
    query.bindValue(":password", password);

    if(query.exec())
    {
        qDebug() << "Data were written to the db\n";
    }
    else
    {
        qDebug() << "Data failed to write to the db\n";
    }
}

void DatabaseController::addFile(const QUuid& uuid, const QString& fileName, const QString& filePath, const QString& userId)
{
    if(!m_database.isOpen())
    {
        qDebug() << "addFile failed to execute. database connection is not opened!\n";
    }

    QSqlQuery query;
    query.prepare("INSERT INTO files (file_id, file_name, file_path, user_id) VALUES (:fileid, :filename, :filepath, :userid)");
    query.bindValue(":fileid", uuid.toString());
    query.bindValue(":filename", fileName);
    query.bindValue(":filepath", filePath);
    query.bindValue(":userid", userId);

    if (query.exec())
    {
        qDebug() << "Data inserted successfully\n";
    }
    else
    {
        qDebug() << "Failed to insert data:" << query.lastError().text() + "\n";
    }
}

QUuid DatabaseController::userExist(const QString& userName, const QString& password)
{
    if(!m_database.isOpen())
    {
        qDebug() << "userExist failed to execute. database connection is not opened!\n";
        return QUuid();
    }

    QSqlQuery query;
    query.prepare("SELECT uuid FROM user_id WHERE user_name = :user_name AND password = :password");
    query.bindValue(":user_name", userName);
    query.bindValue(":password", password);

    if (query.exec() && query.next())
    {
        qDebug() << "found user\n";
        QString user_id = query.value(0).toString();
        return QUuid(user_id);
    }

    qDebug() << "user didn't find\n";
    return QUuid();
}

bool DatabaseController::fileExist(const QString& fileName, const QString& filePath, const QString& userId)
{
    if(!m_database.isOpen())
    {
        qDebug() << "addFile failed to execute. database connection is not opened!\n";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM files WHERE file_name = :fileName AND file_path = :filePath AND user_id = :userId");
    query.bindValue(":fileName", fileName);
    query.bindValue(":filePath", filePath);
    query.bindValue(":userId", userId);

    if (query.exec() && query.next())
    {
        const int count = query.value(0).toInt();

        if (count > 0)
        {
            return true;
        }
        return false;
    }
    else
    {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return false;
    }
}

DatabaseController::~DatabaseController()
{

}
