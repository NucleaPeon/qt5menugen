#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btncheck, SIGNAL(clicked()), this, SLOT(checkShortcut()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkShortcut()
{
    const QString shortcut = ui->txtshortcut->text();
    QMap<QString, int> shortcuts = QtMenuGen::load_shortcuts();
    QStringList lst = shortcut.split("+");
    if (lst.size() > 4) {
        QMessageBox msgBox;
        QString msg = QString("A shortcut can only contain up to 4 key presses (limitation set by qt)");
        msgBox.setText(msg);
        msgBox.exec();
        return;
    }
    bool valid = true;
    foreach(const QString s, lst) {
        qDebug() << shortcuts.size();
        if (shortcuts.value(s, 0) == 0) {
            QMessageBox msgBox;
            QString msg = QString("The shortcut %1 is not recognized by qt5menugen").arg(s);
            msgBox.setText(msg);
            msgBox.exec();
            valid = false;
        }
    }
    if (valid) {
        QMessageBox msgBox;
        QString msg = QString("%1 is a valid shortcut!").arg(shortcut);
        msgBox.setText(msg);
        msgBox.exec();
    }

}
