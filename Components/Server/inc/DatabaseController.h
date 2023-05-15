
#ifndef SERVER_DATABASECONTROLLER_H
#define SERVER_DATABASECONTROLLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


class DatabaseController : public QObject
{
    Q_OBJECT
public:
    DatabaseController(const QString& host, const std::uint16_t port, const QString& databaseName,
                       const QString& userName, const QString& password, QObject* parent = nullptr);

    virtual ~DatabaseController();

public:
    void start();
    void stop();

    

public slots:

signals:

private:
    QSqlDatabase m_database;
    QString m_host;
    QString m_databaseName;
    QString m_userName;
    QString m_password;
    std::uint16_t m_port;

};



#endif //SERVER_DATABASECONTROLLER_H
