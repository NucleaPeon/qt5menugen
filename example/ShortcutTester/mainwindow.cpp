#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMap>
#include <QFile>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFile def(":/menu.json");
    QtMenuGen::setupToolBarOn(&def, this, this);
    connect(ui->btncheck, SIGNAL(clicked()), this, SLOT(checkShortcut()));
    QAction* act = QtMenuGen::actionByName("test");
    QMap<QString, int> shortcuts = QtMenuGen::load_shortcuts();
    qDebug() << QKeySequence::Underline;
    qDebug() << Qt::Key_U;
    qDebug() << shortcuts.value("Qt::Key_Control");
    // LEFT OFF
    // TODO: Currently only StandardKey enum conversions work as expected, so ignore Qt::Key for now.
    // Convert QtMenuGen into class structure, not static, release as v2.0.0.
    // Only convert Qt::Key values if we have examples that show it actually working.
    act->setShortcut(QKeySequence((QKeySequence::StandardKey) 29));
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
        QAction* act = QtMenuGen::actionByName("test");
        qDebug() << act;
        QKeySequence seq;
        qDebug() << lst;
        switch(lst.size()) {
            case 1:
                qDebug() << lst.at(0) << shortcuts.value(lst.at(0), 0);
                seq = QKeySequence(shortcuts.value(lst.at(0), 0));
                break;
            case 2:
                qDebug() << lst.at(0) << shortcuts.value(lst.at(0), 0);
                qDebug() << lst.at(1) << shortcuts.value(lst.at(1), 0);
                seq = QKeySequence(shortcuts.value(lst.at(0), 0),
                                   shortcuts.value(lst.at(1), 0));
                break;
            case 3:
                seq = QKeySequence(shortcuts.value(lst.at(0), 0),
                                   shortcuts.value(lst.at(1), 0),
                                   shortcuts.value(lst.at(2), 0));
                break;
            case 4:
                seq = QKeySequence(shortcuts.value(lst.at(0), 0),
                                   shortcuts.value(lst.at(1), 0),
                                   shortcuts.value(lst.at(2), 0),
                                   shortcuts.value(lst.at(3), 0));
                break;

        }
        act->setShortcut(seq);
        qDebug() << seq;
        QMessageBox msgBox;
        QString msg = QString("%1 is a valid shortcut! It is now assigned to the QAction, try it out!").arg(shortcut);
        msgBox.setText(msg);
        msgBox.exec();
        qDebug() << act->shortcut();
    }

}

void MainWindow::itworks()
{
    QMessageBox msgBox;
    msgBox.setText("It works!");
    msgBox.exec();
}
