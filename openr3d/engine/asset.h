#ifndef ASSET_H
#define ASSET_H

#include <string>
#include "aligned.h"

class Asset
{

public:

    enum Type {
        MESH,
        POLYGON,
        TEXTURE,
        MATERIAL,
        NORMAL_MAP
    };

    const Asset::Type type;
    std::string fileName;

    virtual int load(const std::string& fileName);
    virtual void clear();

    virtual ~Asset();
protected:
    Asset(Asset::Type type);

};

#endif // ASSET_H
