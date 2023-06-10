#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QFile>
#include <string>

#include "inc/Server.h"
#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    const QString pathToTheLog = "/home/dany/logs/log1.txt";
    QFile file(pathToTheLog);
    if(file.exists())
    {
        file.remove();
    }

    plog::init(plog::debug, pathToTheLog.toStdString().c_str(), 1000000, 5);

    coursework::Server s;
    s.start();

    return QCoreApplication::exec();
}
