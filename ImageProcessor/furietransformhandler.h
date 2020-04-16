#ifndef FURIETRANSFORMHANDLER_H
#define FURIETRANSFORMHANDLER_H

#include "ihandler.h"

class FurieTransformHandler : IHandler
{
public:
    FurieTransformHandler();
    virtual ~FurieTransformHandler() override = default;

protected:

    void FFT2D();

    void RFFT2D();
};

#endif // FURIETRANSFORMHANDLER_H
