#ifndef FileDataProvider_H
#define FileDataProvider_H

#include "dataprovider.h"

class FileDataProvider : public DataProvider
{
    Q_OBJECT
public:
    FileDataProvider(const QString &filePath);
    virtual ~FileDataProvider() {}

    virtual void ensureDataAvailable() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void loadFile();

private:
    QString m_filePath;
};

#endif // DataProvider_H
