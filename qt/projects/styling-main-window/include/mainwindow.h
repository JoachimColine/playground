#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void buildMainWindow();

private:
    void addCentralWidget();
    void addMenubar();
    void addToolbar();
    void addDockWidgets();

private:
    QWidget* buildCentralWidget();
};