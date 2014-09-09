#ifndef DataProvider_H
#define DataProvider_H

#include <QSharedPointer>

class DataProvider : public QObject
{
    Q_OBJECT
public:
    typedef QSharedPointer<DataProvider> Ptr;
    virtual ~DataProvider() {}

    virtual void ensureDataAvailable() = 0;

    static Ptr createProvider();

Q_SIGNALS:
    void error();
    void dataAvailable(const QJsonDocument &doc);

protected:
    DataProvider() {}
};

#endif // DataProvider_H
