#include <QApplication>
#include <QDebug>
#include <logging/Logger.h>
#include <logging/Log.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    JApp::Logger& logger = JApp::Logger::instance();
    logger.initialize();

    return 0;
}

