#ifndef ASSET_H
#define ASSET_H

#include <string>

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

};

#endif // ASSET_H
