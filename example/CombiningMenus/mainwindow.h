#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <qt5menugen.h>
#include <QtCore/QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void showContextMenu(const QPoint &);

private:
    Ui::MainWindow *ui;

    QtMenuGen *main;
    QtMenuGen *common;


    QMenu* contextMenu;
    QtMenuGen *context;
    QtMenuGen *contextcommon;

};

#endif // MAINWINDOW_H
