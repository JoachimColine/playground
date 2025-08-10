#include "mainwindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(buildCentralWidget());
}

QWidget* MainWindow::buildCentralWidget()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    mainLayout->addWidget(new QLabel("QLabel"));
    mainLayout->addWidget(new QPushButton("QPushButton"));
    mainLayout->addWidget(new QLineEdit("QLineEdit"));
    mainLayout->addWidget(new QCheckBox("QCheckBox"));
    mainLayout->addWidget(new QRadioButton("QRadioButton"));

    QComboBox *combo = new QComboBox;
    combo->addItems({"Combo Item 1", "Combo Item 2", "Combo Item 3"});
    mainLayout->addWidget(combo);

    mainLayout->addWidget(new QSpinBox);
    mainLayout->addWidget(new QSlider(Qt::Horizontal));
    mainLayout->addWidget(new QProgressBar);

    QTextEdit *textEdit = new QTextEdit("QTextEdit");
    mainLayout->addWidget(textEdit);

    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->addTab(new QLabel("Tab 1"), "Tab 1");
    tabWidget->addTab(new QLabel("Tab 2"), "Tab 2");
    mainLayout->addWidget(tabWidget);

    QGroupBox *group = new QGroupBox("QGroupBox");
    QHBoxLayout *groupLayout = new QHBoxLayout(group);
    groupLayout->addWidget(new QPushButton("In Group"));
    groupLayout->addWidget(new QLineEdit("In Group"));
    mainLayout->addWidget(group);

    QTableWidget *table = new QTableWidget(3, 3);
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            table->setItem(row, col, new QTableWidgetItem(QString("Item %1,%2").arg(row).arg(col)));
    mainLayout->addWidget(table);

    mainLayout->addWidget(new QDateEdit(QDate::currentDate()));
    mainLayout->addWidget(new QTimeEdit(QTime::currentTime()));

    QListWidget *listWidget = new QListWidget;
    listWidget->addItems({"List Item 1", "List Item 2", "List Item 3"});
    mainLayout->addWidget(listWidget);

    return central;
}
