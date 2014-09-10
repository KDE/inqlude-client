/*  This file is part of inqludeclient
 *  Copyright 2014  David Faure  <faure@kde.org>
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License or ( at
 *  your option ) version 3 or, at the discretion of KDE e.V. ( which shall
 *  act as a proxy as in section 14 of the GPLv3 ), any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "httpdataprovider.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <iostream>

HttpDataProvider::HttpDataProvider(const QString &cacheFile)
    : m_qnam(new QNetworkAccessManager(this)),
      m_cacheFile(cacheFile)
{
}

HttpDataProvider::~HttpDataProvider()
{
}

void HttpDataProvider::ensureDataAvailable()
{
    std::cerr << "Downloading library information, please wait..." << std::endl;
    QUrl url("http://inqlude.org/inqlude-all.json");
    QNetworkRequest request(url);
    QNetworkReply* reply = m_qnam->get(request);
    connect(m_qnam, &QNetworkAccessManager::finished, this, &HttpDataProvider::slotFinished);
}

void HttpDataProvider::slotFinished(QNetworkReply *reply)
{
    const QByteArray data = reply->readAll(); // QJsonDocument can't read from a QIODevice !
    reply->deleteLater();
    const QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        emit error();
        return;
    }
    // Write out cache
    QDir().mkpath(QFileInfo(m_cacheFile).absolutePath());
    QFile file(m_cacheFile);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
    }
    emit dataAvailable(doc);
}
