#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#ifdef Q_OS_MAC
    QStringList paths = QStringList() << ":/toolbar" << ":/common";
	this->main = new QtMenuGen(paths);
	qWarning("----------------------------------------------------------------------------");
	qWarning("OS X + qtmenugen does not yet allow updating toolbars after setup.");
	qWarning("You won't get an error, but if the user wants the toolbar set up, they");
	qWarning("will have to customize it themselves until this is fixed. Currently using a");
	qWarning("multi-file constructor and a Q_OS_MAC specific directive can overcome this.");
	qWarning("----------------------------------------------------------------------------");
#else
	this->main = new QtMenuGen(":/toolbar");
    this->common = new QtMenuGen(":/common");
#endif

    this->main->setup(this, this);

#ifndef Q_OS_MAC
    this->main->update(this->common, this, QtMenuGen::MENUBAR);
    this->main->update(this->common, this, QtMenuGen::TOOLBAR);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}
