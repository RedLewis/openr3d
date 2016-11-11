#ifndef LIGHTING_H
#define LIGHTING_H

#include "aligned.h"
#include "color.h"

class Lighting : public Aligned<Alignment::SSE>
{

public:

    Color ambientColor;

    Lighting();

};

#endif // LIGHTING_H
