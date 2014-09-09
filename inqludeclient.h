#ifndef InqludeClient_H
#define InqludeClient_H

#include "dataprovider.h"
#include <QtCore/QObject>

class InqludeClient : public QObject
{
    Q_OBJECT

public:
    InqludeClient();
    virtual ~InqludeClient();

    int run();

private Q_SLOTS:
    void list(const QJsonDocument &doc);

};

#endif // InqludeClient_H
