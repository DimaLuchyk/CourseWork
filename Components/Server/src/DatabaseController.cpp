#include <QDebug>

#include "DatabaseController.h"
#include "plog/Log.h"

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
    PLOG_DEBUG << "DatabaseController ctor";

    m_database = QSqlDatabase::addDatabase("QPSQL");
    m_database.setHostName(m_host);
    m_database.setPort(m_port);
    m_database.setDatabaseName(m_databaseName);
    m_database.setUserName(m_userName);
    m_database.setPassword(m_password);

    PLOG_INFO << "DatabaseController settings: {HOST: " << host.toStdString().c_str() <<
              ", PORT: " << port << ", databaseName: " << databaseName.toStdString().c_str() <<
              ", UserName: " << userName.toStdString().c_str() << ", Password: " << password.toStdString().c_str() << "}";
}

DatabaseController::~DatabaseController()
{
    PLOG_DEBUG << "DabaseController dtor";
}

void DatabaseController::start()
{
    if (m_database.open())
    {
        PLOG_INFO << "DatabaseController connected to the database";
    }
    else
    {
        PLOG_ERROR << "DatabaseController failed to connect to the database. Error:" << m_database.lastError().text().toStdString().c_str();
    }

}

void DatabaseController::stop()
{
    if(m_database.isOpen())
    {
        m_database.close();
        PLOG_INFO << "Database connection was closed.";
    }
}

void DatabaseController::addUser(const QUuid& uuid, const QString& userName, const QString& password)
{
    if(!m_database.isOpen())
    {
        PLOG_ERROR << "Database connection is not opened";
        return;
    }

    QSqlQuery query(m_database);
    query.prepare("INSERT INTO users (user_id, user_name, password) VALUES (:userid, :username, :password);");
    auto uuidStr = uuid.toString();
    query.bindValue(":userid", uuidStr.mid(1, uuidStr.length() - 2));
    query.bindValue(":username", userName);
    query.bindValue(":password", password);

    if(query.exec())
    {
        PLOG_INFO << "User was added to the database. User {user_id: " << uuidStr.toStdString().c_str() << ", user_name: " << userName << ", password: " << password << "}";
    }
    else
    {
        PLOG_ERROR << "Failed to add user to the database. User {user_id: " << uuidStr.toStdString().c_str() << ", user_name: " << userName << ", password: " << password << "}";;
    }

}

void DatabaseController::addFile(const QUuid& uuid, const QString& fileName, const QString& filePath, const QString& userId)
{
    if(!m_database.isOpen())
    {
        qDebug() << "addFile failed to execute. database connection is not opened!\n";
        return;
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

QUuid DatabaseController::userExist(const QString& userName)
{
    if(!m_database.isOpen())
    {
        PLOG_ERROR << "Database connection is not opened";
        return QUuid{};
    }

    QSqlQuery query;
    query.prepare("SELECT user_id FROM users WHERE user_name = :user_name");
    query.bindValue(":user_name", userName);

    if (query.exec() && query.next())
    {
        PLOG_INFO << "There's user with such name. Name: " << userName;
        QString user_id = query.value(0).toString();
        return QUuid(user_id);
    }

    PLOG_INFO << "There's no user with such name. Name: " << userName;
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
