#include "asset.h"

Asset::Asset(Asset::Type type)
    : type(type)
{
}

Asset::~Asset()
{
}

int Asset::load(const std::string& fileName)
{
    this->fileName = fileName;
    return (0);
}
