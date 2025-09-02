#include <QApplication>
#include <QDebug>
#include <logging/Logger.h>
#include <logging/Log.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    JApp::Logger& logger = JApp::Logger::instance();
    logger.initialize();
    std::cout << QString(CURRENT_LOG_CATEGORY().categoryName()).toStdString() << std::endl;

    return 0;
}

