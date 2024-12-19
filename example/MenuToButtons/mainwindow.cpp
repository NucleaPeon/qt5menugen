#include "mainwindow.h"
#include ""

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	this->main = new QtMenuGen(":/toolbar");
    this->main->setup(this, this);

    this->ui->centralWidget->add
}

MainWindow::~MainWindow()
{
    delete ui;
}
