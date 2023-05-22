#include <QCoreApplication>
#include <QDebug>
#include <iostream>

#include "inc/Server.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Create the file sink that all loggers will share
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("/home/dany/logs/basic-log.txt", true);

    // Create logger 1 and attach the file sink
    auto logger1 = std::make_shared<spdlog::logger>("logger1", file_sink);
    logger1->set_level(spdlog::level::debug);
    spdlog::register_logger(logger1);

    // Create logger 2 and attach the file sink
    auto logger2 = std::make_shared<spdlog::logger>("logger2", file_sink);
    logger2->set_level(spdlog::level::debug);
    spdlog::register_logger(logger2);

    // Log messages using logger 1
    logger1->info("This is logger 1");
    logger1->debug("Debug message from logger 1");


    // Log messages using logger 2
    logger2->info("This is logger 2");
    logger2->debug("Debug message from logger 2");


    // Flush all loggers and close the file sink
    spdlog::shutdown();


    coursework::Server s;

    return QCoreApplication::exec();
}
