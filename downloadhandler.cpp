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

#include "downloadhandler.h"

#include <QCoreApplication>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

DownloadHandler::DownloadHandler(QTextStream &errorStream, const QString &library)
    : m_errorStream(errorStream), m_library(library), m_reply(Q_NULLPTR), m_destFile(Q_NULLPTR)
{
}

DownloadHandler::~DownloadHandler()
{
    m_errorStream.flush();
    delete m_destFile;
    delete m_reply;
}

void DownloadHandler::download(const QJsonDocument &doc)
{
    bool libraryFound = false;
    const QJsonArray libs = doc.array();
    for (int i = 0; i < libs.count(); ++i) {
        const QJsonObject details = libs.at(i).toObject();
        const QString name = details.value("name").toString();
        if (name == m_library) {
            const QJsonObject packages = details.value("packages").toObject();
            const QString source = packages.value("source").toString();
            const QUrl sourceUrl(source);
            m_errorStream << "Downloading " << sourceUrl.toDisplayString() << "...\n";
            startDownload(sourceUrl);
            libraryFound = true;
        }
    }
    if (!libraryFound) {
        m_errorStream << "Library " << m_library << " not found\n";
        handlingCompleted();
    }
}

void DownloadHandler::startDownload(const QUrl &sourceUrl)
{
    m_destFile = new QFile(sourceUrl.fileName());
    if (!m_destFile->open(QIODevice::WriteOnly)) {
        m_errorStream << "Cannot write to " << m_destFile->fileName() << "\n";
        handlingCompleted();
        return;
    }
    QNetworkAccessManager *qnam = new QNetworkAccessManager(this);
    QNetworkRequest request(sourceUrl);
    m_reply = qnam->get(request);
    connect(m_reply, &QNetworkReply::readyRead, this, &DownloadHandler::slotReadyRead);
    connect(qnam, &QNetworkAccessManager::finished, this, &DownloadHandler::slotFinished);
}

void DownloadHandler::slotReadyRead()
{
    m_destFile->write(m_reply->readAll());
}

void DownloadHandler::slotFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        m_errorStream << reply->errorString() << '\n';
    }
    m_destFile->close();
    handlingCompleted();
}
