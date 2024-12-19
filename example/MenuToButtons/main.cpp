#include "mainwindow.h"
#include <QtWidgets/QApplication>

#ifdef Q_OS_MAC
#include "cocoainitializer.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_OS_MAC
    CocoaInitializer initializer;
#endif
    MainWindow w;
    w.show();

    return a.exec();
}
