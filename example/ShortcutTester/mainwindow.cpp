#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->menugen = new QtMenuGen(":/menu.json");
    this->menugen->setup(this, this);
    this->shortcuts = this->menugen->getShortcuts();
   connect(ui->btncheck, SIGNAL(clicked()), this, SLOT(checkShortcut()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkShortcut()
{
    const QString shortcut = ui->txtshortcut->text();
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
        if (shortcuts.value(s, 0) == 0) {
            QMessageBox msgBox;
            QString msg = QString("The shortcut %1 is not recognized by qt5menugen").arg(s);
            msgBox.setText(msg);
            msgBox.exec();
            valid = false;
        }
    }
    if (valid) {
        QAction* act = this->menugen->actionByName("test");
        QKeySequence seq;
        switch(lst.size()) {
            case 1:
                if (shortcuts.contains(lst.at(0)))
                    qDebug() << "setting seq" << shortcuts.value(lst.at(0));
                    seq = QKeySequence((QKeySequence::StandardKey) shortcuts.value(lst.at(0)));
                break;
            case 2:
                if (shortcuts.contains(lst.at(0)) && shortcuts.contains(lst.at(1)))
                    seq = QKeySequence((int) shortcuts.value(lst.at(0)),
                                       (int) shortcuts.value(lst.at(1)));
                break;
            case 3:
                if (shortcuts.contains(lst.at(0)) && shortcuts.contains(lst.at(1))
                        && shortcuts.contains(lst.at(2)))
                    seq = QKeySequence((int) shortcuts.value(lst.at(0)),
                                       (int) shortcuts.value(lst.at(1)),
                                       (int) shortcuts.value(lst.at(2)));
                break;
            case 4:
                if (shortcuts.contains(lst.at(0)) && shortcuts.contains(lst.at(1))
                        && shortcuts.contains(lst.at(2)) && shortcuts.contains(lst.at(3)))
                    seq = QKeySequence((int) shortcuts.value(lst.at(0)),
                                       (int) shortcuts.value(lst.at(1)),
                                       (int) shortcuts.value(lst.at(2)),
                                       (int) shortcuts.value(lst.at(3)));
                break;

        }
        qDebug() << act << seq;
        if(act != NULL) {
            qDebug() << "Setting shortcut to " << seq;
            act->setShortcut(seq);
            QMessageBox msgBox;
            QString msg = QString("%1 is a valid shortcut! It is now assigned to the QAction, try it out!").arg(shortcut);
            msgBox.setText(msg);
            msgBox.exec();
        } else {
            QMessageBox msgBox;
            msgBox.setText("QAction not found / is null");
            msgBox.exec();
        }
    }

}

void MainWindow::itworks()
{
    QMessageBox msgBox;
    msgBox.setText("It works!");
    msgBox.exec();
}
