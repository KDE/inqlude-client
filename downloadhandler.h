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

#ifndef DownloadHandler_H
#define DownloadHandler_H

#include "abstracthandler.h"
#include <QUrl>

class QTextStream;
class QJsonDocument;
class QNetworkReply;
class QNetworkAccessManager;
class QFile;

class DownloadHandler : public AbstractHandler
{
    Q_OBJECT
public:
    DownloadHandler(QTextStream &errorStream, const QString &library);
    ~DownloadHandler();


public Q_SLOTS:
    void download(const QJsonDocument &doc);

protected:
    virtual void startDownload(const QUrl &sourceUrl); // reimplemented by the unittest

private Q_SLOTS:
    void slotReadyRead();
    void slotFinished(QNetworkReply *reply);

private:
    QTextStream& m_errorStream;
    QString m_library;
    QNetworkReply *m_reply;
    QFile *m_destFile;
    QUrl m_urlRedirect;
    QNetworkAccessManager *m_qnam;
};

#endif
