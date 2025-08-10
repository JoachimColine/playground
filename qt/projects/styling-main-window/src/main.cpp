#include "mainwindow.h"
#include <QApplication>
#include <QFile>

QString loadStyleSheet()
{
    QFile styleFile(":/style.qss");
    styleFile.open(QFile::ReadOnly);
    return QLatin1String(styleFile.readAll());
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(loadStyleSheet());
    MainWindow window;
    window.show();
    return app.exec();
}

