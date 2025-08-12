#include "mainwindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QProgressBar>
#include <QTextEdit>
#include <QTabWidget>
#include <QGroupBox>
#include <QTableWidget>
#include <QDateEdit>
#include <QTimeEdit>
#include <QListWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QCalendarWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    buildMainWindow();
}

void MainWindow::buildMainWindow()
{
    addCentralWidget();
    addMenubar();
    addToolbar();
    addDockWidgets();
    setMinimumSize(1200, 400);
}

void MainWindow::addCentralWidget()
{
    setCentralWidget(buildCentralWidget());
}

void MainWindow::addMenubar()
{
    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("New");
    fileMenu->addAction("Open");
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QMainWindow::close);

    QMenu *editMenu = menuBar()->addMenu("Edit");
    editMenu->addAction("Cut");
    editMenu->addAction("Copy");
    editMenu->addAction("Paste");

    QMenu *helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction("About");
}

void MainWindow::addToolbar()
{
    QToolBar *toolbar = addToolBar("Main Toolbar");
    toolbar->addAction("New");
    toolbar->addAction("Open");
    toolbar->addAction("Save");
    toolbar->addSeparator();
    toolbar->addAction("Cut");
    toolbar->addAction("Copy");
    toolbar->addAction("Paste");
}

void MainWindow::addDockWidgets()
{
    QDockWidget *dock1 = new QDockWidget("Dock Widget 1", this);
    dock1->setWidget(new QTextEdit("Docked QTextEdit"));
    addDockWidget(Qt::LeftDockWidgetArea, dock1);

    QDockWidget *dock2 = new QDockWidget("Dock Widget 2", this);
    QListWidget *list = new QListWidget;
    list->addItems({"Dock Item 1", "Dock Item 2", "Dock Item 3"});
    dock2->setWidget(list);
    addDockWidget(Qt::RightDockWidgetArea, dock2);

    QDockWidget *dock3 = new QDockWidget("Dock Widget 3", this);
    addDockWidget(Qt::BottomDockWidgetArea, dock3);
}


QWidget* MainWindow::buildCentralWidget()
{
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    QVBoxLayout *leftLayout = new QVBoxLayout(central);
    QVBoxLayout *rightLayout = new QVBoxLayout(central);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    leftLayout->addWidget(new QLabel("QLabel"));
    leftLayout->addWidget(new QPushButton("QPushButton"));
    leftLayout->addWidget(new QLineEdit("QLineEdit"));
    leftLayout->addWidget(new QCheckBox("QCheckBox"));
    leftLayout->addWidget(new QRadioButton("QRadioButton"));

    QComboBox *combo = new QComboBox;
    combo->addItems({"Combo Item 1", "Combo Item 2", "Combo Item 3"});
    leftLayout->addWidget(combo);

    leftLayout->addWidget(new QSpinBox);

    auto slider = new QSlider(Qt::Horizontal);
    auto progressBar = new QProgressBar;
    connect(slider, &QSlider::valueChanged, progressBar, &QProgressBar::setValue);

    leftLayout->addWidget(slider);
    leftLayout->addWidget(progressBar);
    leftLayout->addWidget(new QTextEdit);

    QTabWidget *tabWidget = new QTabWidget;

    QWidget *tab1 = new QWidget;
    QGridLayout *gridLayout = new QGridLayout(tab1);

    gridLayout->addWidget(new QLabel("Name:"), 0, 0);
    gridLayout->addWidget(new QLineEdit, 0, 1);

    gridLayout->addWidget(new QLabel("Password:"), 1, 0);
    QLineEdit *passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    gridLayout->addWidget(passwordEdit, 1, 1);

    gridLayout->addWidget(new QPushButton("Login"), 2, 0);
    gridLayout->addWidget(new QPushButton("Cancel"), 2, 1);

    gridLayout->addWidget(new QCheckBox("Remember me"), 3, 0, 1, 2);

    gridLayout->addWidget(new QLabel("Options:"), 4, 0);
    QComboBox *optionsCombo = new QComboBox;
    optionsCombo->addItems({"Option A", "Option B", "Option C"});
    gridLayout->addWidget(optionsCombo, 4, 1);

    tabWidget->addTab(tab1, "Tab 1");

    QWidget *tab2 = new QWidget;
    QVBoxLayout *tab2Layout = new QVBoxLayout(tab2);
    QCalendarWidget *calendar = new QCalendarWidget;
    tab2Layout->addWidget(calendar);

    tabWidget->addTab(tab2, "Tab 2");

    rightLayout->addWidget(tabWidget);

    QGroupBox *group = new QGroupBox("QGroupBox");
    QHBoxLayout *groupLayout = new QHBoxLayout(group);
    groupLayout->addWidget(new QPushButton("In Group"));
    groupLayout->addWidget(new QLineEdit("In Group"));
    rightLayout->addWidget(group);

    int tableSize = 100;
    QTableWidget *table = new QTableWidget(tableSize, tableSize);
    for (int row = 0; row < tableSize; ++row)
        for (int col = 0; col < tableSize; ++col)
            table->setItem(row, col, new QTableWidgetItem(QString("Item %1,%2").arg(row).arg(col)));
    rightLayout->addWidget(table);

    rightLayout->addWidget(new QDateEdit(QDate::currentDate()));
    rightLayout->addWidget(new QTimeEdit(QTime::currentTime()));

    QListWidget *listWidget = new QListWidget;
    int listSize = 100;
    for (int row = 0; row < tableSize; ++row)
        listWidget->addItem(QString("Item %1").arg(row));
    rightLayout->addWidget(listWidget);

    return central;
}
