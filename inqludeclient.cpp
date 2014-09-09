#include "inqludeclient.h"
#include "dataprovider.h"
#include <QCommandLineParser>

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
        connect(provider.data(), &DataProvider::error, qApp, &QCoreApplication::quit);
        connect(provider.data(), &DataProvider::dataAvailable, this, &InqludeClient::list);
        return qApp->exec();
    }

    return 1;
}

void InqludeClient::list(const QJsonDocument &doc)
{

}

