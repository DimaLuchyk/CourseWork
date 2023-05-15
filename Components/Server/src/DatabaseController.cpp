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

DatabaseController::~DatabaseController()
{

}
