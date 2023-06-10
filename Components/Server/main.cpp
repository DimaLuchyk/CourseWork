#include <QCoreApplication>
#include <QDebug>
#include <iostream>

#include "inc/Server.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);


    coursework::Server s;
    s.start();

    return QCoreApplication::exec();
}
