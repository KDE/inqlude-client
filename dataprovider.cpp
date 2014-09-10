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

#include "dataprovider.h"
#include "filedataprovider.h"
#include "httpdataprovider.h"

#include <QStandardPaths>
#include <QDateTime>
#include <QFileInfo>

DataProvider::Ptr DataProvider::createProvider()
{
    // Check cache
    const QString cache = cacheFile();
    QFileInfo info(cache);
    if (info.exists() && info.lastModified().daysTo(QDateTime::currentDateTime()) < 1) {
        // Exists and is recent, use that
        return Ptr(new FileDataProvider(cacheFile()));
    }

    // Download file
    return Ptr(new HttpDataProvider(cacheFile()));
}

QString DataProvider::cacheFile()
{
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/inqlude-all.json";
}
