#include <QApplication>
#include <QDebug>
#include <JApp/Logger.h>
#include <JApp/Log.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    JApp::Logger& logger = JApp::Logger::instance();
    logger.initialize();
    LOG_DEBUG() << "Debug from main!";
    LOG_INFO() <<  "Info from main!";
    LOG_WARN() <<  "Warn from main!";


    return 0;
}

