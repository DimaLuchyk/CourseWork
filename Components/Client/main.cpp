#include <QApplication>
#include <QPushButton>

#include "inc/LCClient.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    LCClient client;
    client.show();

    return QApplication::exec();
}
