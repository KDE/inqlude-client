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
#include <QTextStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

DownloadHandler::DownloadHandler(QTextStream &stream, const QString &library)
    : m_stream(stream), m_library(library)
{
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
            const QUrl sourceUrl(source);
            m_stream << "Downloading " << sourceUrl.toDisplayString() << "..." << '\n';
            startDownload(sourceUrl);
        }
    }
    m_stream.flush();
    handlingCompleted();
}

void DownloadHandler::startDownload(const QUrl &sourceUrl)
{

}
