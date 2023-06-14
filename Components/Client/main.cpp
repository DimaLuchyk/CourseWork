#include <QApplication>
#include <QObject>

#include "inc/ConnectionWindow.h"
#include "inc/AuthorizationWindow.h"
#include "inc/MainWindow.h"
#include "inc/NetworkClient.h"
#include "../../Server/inc/Shared.h"

#include <memory>


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    auto client = std::make_shared<coursework::NetworkClient>();
    auto connectionWindow = std::make_unique<coursework::windows::ConnectionWindow>(client);
    auto authorizationWindow = std::make_unique<coursework::windows::AuthorizationWindow>(client);
    auto mainWindow = std::make_unique<coursework::windows::MainWindow>(client);

    // Connect signals and slots
    QObject::connect(connectionWindow.get(), &coursework::windows::ConnectionWindow::connectedToServer, authorizationWindow.get(), &coursework::windows::AuthorizationWindow::show);
    QObject::connect(connectionWindow.get(), &coursework::windows::ConnectionWindow::connectedToServer, connectionWindow.get(), &coursework::windows::ConnectionWindow::hide);
    QObject::connect(client.get(), &coursework::NetworkClient::loggedIn, authorizationWindow.get(), &coursework::windows::AuthorizationWindow::hide);
    QObject::connect(client.get(), &coursework::NetworkClient::loggedIn, mainWindow.get(), &coursework::windows::MainWindow::show);
    QObject::connect(client.get(), &coursework::NetworkClient::receievedExistedFiles, mainWindow.get(), &coursework::windows::MainWindow::updateFiles);
    QObject::connect(client.get(), &coursework::NetworkClient::fileDownloaded, mainWindow.get(), &coursework::windows::MainWindow::saveDownloadedFile);
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
