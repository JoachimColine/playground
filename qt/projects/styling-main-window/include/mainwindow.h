#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void setStyleSheet(QString styleSheet);

private:
    void buildMainWindow();

private:
    void addCentralWidget();
    void addMenubar();
    void addToolbar();
    void addDockWidgets();

private slots:
    void onLoadStyleSheetActionTriggered();

private:
    QWidget* buildCentralWidget();
};