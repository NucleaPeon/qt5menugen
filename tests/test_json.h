#ifndef TEST_JSON_H
#define TEST_JSON_H

#include <QtCore/QObject>
#include <QtTest/QtTest>

class  QJsonTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testQJsonValueFromString();
};


#endif // TEST_JSON_H
