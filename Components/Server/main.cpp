#include <QCoreApplication>
#include <QDebug>

#include "Server.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    Server s;

    return QCoreApplication::exec();
}
