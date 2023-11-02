#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->aboutWindow = new About();
    this->preferencesWindow = new Preferences();
    ui->setupUi(this);
    this->statusBar = new QStatusBar();
    this->setStatusBar(this->statusBar);

    // Code to load our menus and toolbars via json:
    QFile pathToJson(":/json");
    QtMenuGen::setupToolBarOn(&pathToJson, this, this);
    // --------------------------------------------

    this->statusBar->showMessage("Ready.");
}

MainWindow::~MainWindow()
{
    delete this->aboutWindow;
    delete this->preferencesWindow;
    delete this->statusBar;
    delete ui;
}

void MainWindow::showAboutModal()
{
    this->aboutWindow->show();
}

void MainWindow::showPreferencesModal()
{
    this->preferencesWindow->show();
}
