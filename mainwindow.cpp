#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : CustomMoveWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    // ui->widget_5->raise();
}

MainWindow::~MainWindow()
{
    delete ui;
}
