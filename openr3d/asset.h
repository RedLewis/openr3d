#ifndef ASSET_H
#define ASSET_H

#include <string>
#include "aligned.h"

class Asset
{

public:

    enum Type {
        MESH,
        TEXTURE,
        MATERIAL
    };

    const Asset::Type type;
    std::string fileName;

    int load(const std::string& fileName);

protected:

    Asset(Asset::Type type);
    virtual ~Asset();

};

#endif // ASSET_H
