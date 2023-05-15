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
    query.prepare("INSERT INTO users (user_id, username, password) VALUES (:user_id :username, :password)");
    query.bindValue(":user_id", uuid.toString());
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
    query.prepare("INSERT INTO files (file_id, filename, file_path, user_id) VALUES (:file_id, :filename, :file_path, :user_id)");
    query.bindValue(":file_id", uuid.toString());
    query.bindValue(":filename", fileName);
    query.bindValue(":file_path", filePath);
    query.bindValue(":user_id", userId);

    if (query.exec())
    {
        qDebug() << "Data inserted successfully\n";
    }
    else
    {
        qDebug() << "Failed to insert data:" << query.lastError().text() + "\n";
    }
}

bool DatabaseController::userExist(const QString& userName, const QString& password)
{
    if(!m_database.isOpen())
    {
        qDebug() << "addFile failed to execute. database connection is not opened!\n";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", userName);
    query.bindValue(":password", password);

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
