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
    const QJsonArray libs = doc.array();
    for (int i = 0; i < libs.count(); ++i) {
        const QJsonObject details = libs.at(i).toObject();
        const QString name = details.value("name").toString();
        if (name == m_library) {
            const QJsonObject packages = details.value("packages").toObject();
            const QString source = packages.value("source").toString();
            if (source.isEmpty()) {
                m_errorStream << "Library " << m_library << " has no source package\n";
                const QJsonObject urls = details.value("urls").toObject();
                const QString vcs = urls.value("vcs").toString();
                if (!vcs.isEmpty()) {
                    m_errorStream << "Suggestion: see " << vcs << " for access to the sources\n";
                }
                handlingCompleted();
            } else {
                const QUrl sourceUrl(source);
                m_errorStream << "Downloading " << sourceUrl.toDisplayString() << "...\n";
                startDownload(sourceUrl);
            }
            return;
        }
    }
    m_errorStream << "Library " << m_library << " not found\n";
    handlingCompleted();
}

void DownloadHandler::startDownload(const QUrl &sourceUrl)
{
    m_destFile = new QFile(sourceUrl.fileName());
    if (!m_destFile->open(QIODevice::WriteOnly)) {
        m_errorStream << "Cannot write to " << m_destFile->fileName() << "\n";
        handlingCompleted();
        return;
    }
    m_qnam = new QNetworkAccessManager(this);
    QNetworkRequest request(sourceUrl);
    m_reply = m_qnam->get(request);
    connect(m_reply, &QNetworkReply::readyRead, this, &DownloadHandler::slotReadyRead);
    connect(m_qnam, &QNetworkAccessManager::finished, this, &DownloadHandler::slotFinished);
}

void DownloadHandler::slotReadyRead()
{
    if (m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute).isNull()) {
        m_destFile->write(m_reply->readAll());
    }
}

void DownloadHandler::slotFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        m_errorStream << reply->errorString() << '\n';
    }

    // Handle redirections
    const QUrl possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (!possibleRedirectUrl.isEmpty() && possibleRedirectUrl != m_urlRedirect) {
        m_urlRedirect = reply->url().resolved(possibleRedirectUrl);
        if (m_urlRedirect.scheme().startsWith("http")) {
            m_errorStream << "Redirected to " << m_urlRedirect.toDisplayString() << "...\n";
            reply->deleteLater();
            m_reply = m_qnam->get(QNetworkRequest(m_urlRedirect));
            connect(m_reply, &QNetworkReply::readyRead, this, &DownloadHandler::slotReadyRead);
            return;
        } else {
            m_errorStream << "Redirection to" << m_urlRedirect.toDisplayString() << "forbidden.\n";
        }
    }
    m_urlRedirect.clear();
    m_destFile->close();
    handlingCompleted();
}
