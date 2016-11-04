#include "asset.h"

Asset::Asset(Asset::Type type)
    : type(type)
{
}

int Asset::load(const std::string& fileName)
{
    this->fileName = fileName;
    return (0);
}
