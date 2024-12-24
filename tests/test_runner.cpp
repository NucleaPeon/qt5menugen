#include <QtTest/QtTest>
#include <QtCore/QDebug>
#include <QtCore/QObject>

#include "test_qwidget_menu.h"
#include "test_json.h"

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    int result = 0;
    QWidgetMenuTest *qwidgettests = new  QWidgetMenuTest();
    result += QTest::qExec(qwidgettests);
    QJsonTest *qjsontests = new  QJsonTest();
    result += QTest::qExec(qjsontests);

    return result;
}
