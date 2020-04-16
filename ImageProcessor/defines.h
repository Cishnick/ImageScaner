#ifndef DEFINES_H
#define DEFINES_H

#include <functional>
#include <algorithm>
#include <vector>
#include <list>
#include <thread>
#include "totaldefines.h"
#define USE_MATH_DEFINES
#include "math.h"

using funcImage_t = std::function<void(image_t const&)>;
using vbyte_t = std::vector<uint8_t>;


struct optionFilters
{
    virtual ~optionFilters() = default;
};

struct optionFilterMedian : optionFilters
{
    int windowSize;
};

struct optionFilterBilateral : optionFilters
{
    int windowSize;
    double sigmaI;
    double sigmaR;
};

#endif // DEFINES_H
