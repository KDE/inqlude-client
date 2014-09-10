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
#include "commandhandler.h"

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

    const QStringList args = parser.positionalArguments();
    const QString command = args.isEmpty() ? QString() : args.first();
    if (command == QLatin1String("list")) {
        DataProvider::Ptr provider = DataProvider::createProvider();
        provider->ensureDataAvailable();
        connect(provider.data(), &DataProvider::error, this, &InqludeClient::error);
        connect(provider.data(), &DataProvider::dataAvailable, this, &InqludeClient::list);
        return qApp->exec();
    }

    parser.showHelp();
    return 1;
}

void InqludeClient::list(const QJsonDocument &doc)
{
    QTextStream stream(stdout);
    CommandHandler handler(stream);
    handler.list(doc);
    qApp->quit();
}

void InqludeClient::error()
{
    qApp->exit(1);
}

