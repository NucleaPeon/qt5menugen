#include "test_json.h"

void QJsonTest::initTestCase()
{
}


void QJsonTest::testQJsonValueFromString()
{
    const QString jsonstring = "{\"separator\": true}";
    QJsonDocument doc = QJsonDocument::fromJson(jsonstring.toLatin1());
    QVERIFY(! doc.object().isEmpty());
}
