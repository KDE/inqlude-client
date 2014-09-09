#include <QCoreApplication>
#include "inqludeclient.h"


int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setApplicationVersion("1.0");

    InqludeClient inqludeclient;
    return inqludeclient.run();
}
