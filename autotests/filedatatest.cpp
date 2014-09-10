#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>

#include <QTest>
#include <QSignalSpy>

#include "filedataprovider.h"
#include "commandhandler.h"

class TestFileData : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void fileDataProviderShouldProvideData();
    void listCommandShouldOutputList();

private:
    void fetchData(QJsonDocument *doc);
};

void TestFileData::fetchData(QJsonDocument *doc)
{
    DataProvider::Ptr provider(new FileDataProvider(QFINDTESTDATA("inqlude-all.json")));
    provider->ensureDataAvailable();
    QSignalSpy spyError(provider.data(), SIGNAL(error()));
    QSignalSpy spyDataAvailable(provider.data(), SIGNAL(dataAvailable(QJsonDocument)));

    QVERIFY(spyDataAvailable.wait(1000));
    QVERIFY(spyError.isEmpty());
    *doc = spyDataAvailable.at(0).at(0).value<QJsonDocument> ();
    QCOMPARE(doc->array().size(), 3);
}

void TestFileData::fileDataProviderShouldProvideData()
{
    QJsonDocument doc;
    fetchData(&doc);
}

void TestFileData::listCommandShouldOutputList()
{
    QJsonDocument doc;
    fetchData(&doc);

    QString output;
    QTextStream stream(&output);
    CommandHandler handler(stream);
    handler.list(doc);
    const QStringList expected = QStringList()
       << "karchive   Reading, creating, and manipulating file archives"
       << "kauth      Execute actions as privileged user"
       << "kbookmarks Web browser bookmark management"
       << "";
    QCOMPARE(output.split('\n'), expected);
}

QTEST_MAIN(TestFileData)

#include "filedatatest.moc"
