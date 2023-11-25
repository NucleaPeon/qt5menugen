#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QAction>
#include <QMap>
#include <QFile>
#include <QShortcut>

#include <qt5menugen.h>



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
    void checkShortcut();
    void itworks();

private:
    Ui::MainWindow *ui;
    QtMenuGen *menugen;
    QMap<QString, int> shortcuts;
};

#endif // MAINWINDOW_H
