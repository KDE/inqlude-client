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

#include "inqludeclient.h"
#include "dataprovider.h"
#include "downloadhandler.h"
#include "listhandler.h"

#include <QCommandLineParser>
#include <QDebug>

#include <iostream>

InqludeClient::InqludeClient()
{
}

InqludeClient::~InqludeClient()
{}

int InqludeClient::run()
{
    DataProvider::Ptr provider = DataProvider::createProvider();

    QCommandLineParser parser;
    parser.setApplicationDescription(tr("Command-line client for the inqlude.org repository of Qt libraries"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("command", tr("Command: \n"
                "\tlist               List libraries\n"
                "\tdownload <lib>     Download a given library\n"));
    // This leads to --list or -list, it would be nice for QCommandLineParser to support "list" too, but how
    // would it know where to stop parsing options? I guess this would require builtin support for "one-word commands" then.
    //QCommandLineOption optList("list", tr("List libraries"));
    //parser.addOption(optList);

    parser.process(*qApp);

    QTextStream stream(stdout);

    const QStringList args = parser.positionalArguments();
    const QString command = args.isEmpty() ? QString() : args.first();
    if (command == QLatin1String("list")) {
        ListHandler handler(stream);
        handler.setQuitOnCompletion(true);
        connect(provider.data(), &DataProvider::dataAvailable, &handler, &ListHandler::list);
        ensureDataAvailable(provider);
        return qApp->exec();
    } else if (command == QLatin1String("download")) {
        if (args.count() < 2) {
            parser.showHelp(1);
        }
        const QString library = args.at(1);
        DownloadHandler handler(stream, library);
        handler.setQuitOnCompletion(true);
        connect(provider.data(), &DataProvider::dataAvailable, &handler, &DownloadHandler::download);
        ensureDataAvailable(provider);
        return qApp->exec();
    }

    parser.showHelp(1);
    return 1;
}

void InqludeClient::error()
{
    qApp->exit(1);
}

void InqludeClient::ensureDataAvailable(DataProvider::Ptr provider)
{
    provider->ensureDataAvailable();
    connect(provider.data(), &DataProvider::error, this, &InqludeClient::error);
}
