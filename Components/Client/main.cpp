#include <QApplication>
#include <QObject>

#include "inc/ConnectionWindow.h"
#include "inc/AuthorizationWindow.h"
#include "inc/MainWindow.h"
#include "inc/Client.h"
#include "../../Server/inc/Shared.h"

#include <memory>


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    auto client = std::make_shared<coursework::Client>();
    auto connectionWindow = std::make_unique<coursework::windows::ConnectionWindow>(client);
    auto authorizationWindow = std::make_unique<coursework::windows::AuthorizationWindow>(client);

    // Connect signals and slots
    QObject::connect(connectionWindow.get(), &coursework::windows::ConnectionWindow::connectedToServer, authorizationWindow.get(), &coursework::windows::AuthorizationWindow::show);
    QObject::connect(connectionWindow.get(), &coursework::windows::ConnectionWindow::connectedToServer, connectionWindow.get(), &coursework::windows::ConnectionWindow::hide);
    //client->connect("127.0.0.1", 9999);

    //coursework::protocol::AuthorizationPayload payload{"Dmytro", "Luchyk"};
    //auto header = coursework::protocol::PacketGenerator::generatePacketHeader(coursework::protocol::PacketType::LOG_UP, sizeof(payload));
    //client->sendData(coursework::protocol::PacketGenerator::combineToPacket(header, payload));

    // Show the Connection window
    connectionWindow->show();
/*QWidget w;
w.show();*/

    return a.exec();
}
