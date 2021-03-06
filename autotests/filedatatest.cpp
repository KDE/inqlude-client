#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>

#include <QTest>
#include <QSignalSpy>

#include "filedataprovider.h"
#include "listhandler.h"
#include "downloadhandler.h"

class TestFileData : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void fileDataProviderShouldProvideData();
    void listCommandShouldOutputList();
    void invalidDownloadCommandShouldError();
    void downloadCommandWithNoSourceShouldError();
    void downloadCommandShouldStartDownload();

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
    ListHandler handler(stream);
    handler.list(doc);
    const QStringList expected = QStringList()
       << "karchive   Reading, creating, and manipulating file archives"
       << "kauth      Execute actions as privileged user"
       << "kbookmarks Web browser bookmark management"
       << "";
    QCOMPARE(output.split('\n'), expected);
}

void TestFileData::invalidDownloadCommandShouldError()
{
    QJsonDocument doc;
    fetchData(&doc);

    QString output;
    QTextStream stream(&output);
    DownloadHandler handler(stream, "doesnotexist");
    handler.download(doc);
    const QString expected("Library doesnotexist not found\n");
    QCOMPARE(output, expected);
}

void TestFileData::downloadCommandWithNoSourceShouldError()
{
    QJsonDocument doc;
    fetchData(&doc);

    QString output;
    QTextStream stream(&output);
    DownloadHandler handler(stream, "kbookmarks");
    handler.download(doc);
    const QString expected("Library kbookmarks has no source package\nSuggestion: see https://projects.kde.org/projects/frameworks/kbookmarks/repository for access to the sources\n");
    QCOMPARE(output, expected);
}

void TestFileData::downloadCommandShouldStartDownload()
{
    QJsonDocument doc;
    fetchData(&doc);

    QString output;
    QTextStream stream(&output);
    DownloadHandler handler(stream, "kauth");
    handler.download(doc);
    const QString expected("Downloading http://download.kde.org/stable/frameworks/5.1.0/kauth-5.1.0.tar.xz...\n");
    QCOMPARE(output, expected);
}

QTEST_MAIN(TestFileData)

#include "filedatatest.moc"
