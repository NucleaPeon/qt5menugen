#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	this->main = new QtMenuGen(":/toolbar");
    this->main->setup(this, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
