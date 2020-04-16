#ifndef IHANDLER_H
#define IHANDLER_H
#include "defines.h"

class IHandler
{
public:
    virtual void handle(image_t& img) = 0;

    virtual const char* getName() const = 0;

    virtual ~IHandler() = default;
};

#endif // IHANDLER_H
