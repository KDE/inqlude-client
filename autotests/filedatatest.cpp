#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>

#include <QTest>
#include <QSignalSpy>

#include "filedataprovider.h"

class TestFileData : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void fileDataProviderShouldProvideData();

};

void TestFileData::fileDataProviderShouldProvideData()
{
    DataProvider::Ptr provider(new FileDataProvider(QFINDTESTDATA("inqlude-all.json")));
    provider->ensureDataAvailable();
    QSignalSpy spyError(provider.data(), SIGNAL(error()));
    QSignalSpy spyDataAvailable(provider.data(), SIGNAL(dataAvailable(QJsonDocument)));

    QVERIFY(spyDataAvailable.wait(1000));
    QVERIFY(spyError.isEmpty());
    QJsonDocument doc = spyDataAvailable.at(0).at(0).value<QJsonDocument> ();
    QCOMPARE(doc.array().size(), 3);
}

QTEST_MAIN(TestFileData)

#include "filedatatest.moc"
