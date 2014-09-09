#include "filedataprovider.h"
#include <QFile>
#include <QJsonDocument>
#include <QDebug>

void FileDataProvider::ensureDataAvailable()
{
    QMetaObject::invokeMethod(this, "loadFile", Qt::QueuedConnection);
}

FileDataProvider::FileDataProvider(const QString &filePath)
    : m_filePath(filePath)
{
}

void FileDataProvider::loadFile()
{
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit error();
        return;
    }
    const QByteArray data = file.readAll(); // QJsonDocument can't read from a QIODevice !
    const QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        emit error();
        return;
    }
    emit dataAvailable(doc);
}

