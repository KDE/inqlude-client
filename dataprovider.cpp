#include "dataprovider.h"
#include "filedataprovider.h"

DataProvider::Ptr DataProvider::createProvider()
{
    return Ptr(); // DataProvider::Ptr(new HttpDataProvider);
}
