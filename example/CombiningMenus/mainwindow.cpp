#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->toolbar = new QtMenuGen(":/toolbar");
    this->common = new QtMenuGen(":/common");
    this->toolbar->setup(this, this);
    this->toolbar->update(this->common, this, QtMenuGen::MENUBAR);
}

MainWindow::~MainWindow()
{
    delete ui;
}
