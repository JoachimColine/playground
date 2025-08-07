#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    QLabel *label = new QLabel("Hello, Qt6!", centralWidget);
    layout->addWidget(label);
    setCentralWidget(centralWidget);
    setWindowTitle("Minimal Qt Application");
    resize(800, 600);
}
