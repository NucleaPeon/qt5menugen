#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QAction>

#include "about.h"
#include "preferences.h"

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
    void showAboutModal();
    void showPreferencesModal();

private:
    Ui::MainWindow *ui;
    About *aboutWindow;
    Preferences *preferencesWindow;
    QStatusBar *statusBar;
};

#endif // MAINWINDOW_H
