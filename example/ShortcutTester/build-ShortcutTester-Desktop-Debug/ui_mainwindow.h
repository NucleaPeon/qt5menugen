/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget;
    QFormLayout *formLayout_2;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *txtshortcut;
    QPushButton *btncheck;
    QWidget *tab_4;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(614, 187);
        MainWindow->setUnifiedTitleAndToolBarOnMac(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        verticalLayout_3 = new QVBoxLayout(tab_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        widget = new QWidget(tab_3);
        widget->setObjectName(QStringLiteral("widget"));
        formLayout_2 = new QFormLayout(widget);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QStringLiteral("formLayout"));

        formLayout_2->setLayout(0, QFormLayout::LabelRole, formLayout);

        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label);

        txtshortcut = new QLineEdit(widget);
        txtshortcut->setObjectName(QStringLiteral("txtshortcut"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(txtshortcut->sizePolicy().hasHeightForWidth());
        txtshortcut->setSizePolicy(sizePolicy);
        txtshortcut->setMinimumSize(QSize(200, 0));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, txtshortcut);

        btncheck = new QPushButton(widget);
        btncheck->setObjectName(QStringLiteral("btncheck"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, btncheck);


        verticalLayout_3->addWidget(widget);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        tabWidget->addTab(tab_4, QString());

        verticalLayout_2->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QString());
        label->setText(QApplication::translate("MainWindow", "Shortcut String", 0));
        txtshortcut->setText(QApplication::translate("MainWindow", "Qt::Key_Control+Qt::Key_E", 0));
        txtshortcut->setPlaceholderText(QApplication::translate("MainWindow", "Qt::Key_Control+Qt::Key_Q", 0));
        btncheck->setText(QApplication::translate("MainWindow", "Check", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Tab 1", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "Tab 2", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
