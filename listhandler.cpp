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

#include "listhandler.h"

#include <QCoreApplication>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

ListHandler::ListHandler(QTextStream &stream)
    : m_stream(stream)
{
}

void ListHandler::list(const QJsonDocument &doc)
{
    const QJsonArray libs = doc.array();
    int longestName = 0;
    for (int i = 0; i < libs.count(); ++i) {
        const QJsonObject details = libs.at(i).toObject();
        const QString name = details.value("name").toString();
        longestName = qMax(longestName, name.length());
    }
    for (int i = 0; i < libs.count(); ++i) {
        const QJsonObject details = libs.at(i).toObject();
        const QString name = details.value("name").toString();
        const QString summary = details.value("summary").toString();
        m_stream << qPrintable(name.leftJustified(longestName + 1)) << qPrintable(summary) << '\n';
    }
    m_stream.flush();
    handlingCompleted();
}
