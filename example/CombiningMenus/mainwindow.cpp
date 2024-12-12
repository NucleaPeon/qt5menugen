#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->main = new QtMenuGen(":/toolbar");
    this->common = new QtMenuGen(":/common");
    this->main->setup(this, this);
    // this->main->update(this->common, this, QtMenuGen::MENUBAR);
    // this->main->update(this->common, this, QtMenuGen::TOOLBAR);
}

MainWindow::~MainWindow()
{
    delete ui;
}
