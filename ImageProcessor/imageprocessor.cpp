#include "imageprocessor.h"

ImageProcessor::ImageProcessor() :
    _image(), _resImage()
{
    FilterHandler* med = new FilterMedian(3);
    handlers.push_back(med);
    FilterHandler* conv = new FilterConvolution(
                {
                    -1., -1., -1.,
                    -1.,  8., -1.,
                    -1., -1., -1.
                });

    handlers.push_back(conv);

    FilterHandler* handler = new FilterBilateral(7, 80.0, 25.0);
    handlers.push_back(handler);

    //activeHandlers.push_back(handler);
}

ImageProcessor::~ImageProcessor()
{
    for(auto h : handlers)
    {
        delete h;
    }
}

void ImageProcessor::setImage(const image_t & img)
{
    _image = img;
}

const image_t &ImageProcessor::getImage() const
{
    return _image;
}

void ImageProcessor::setImageFunction(funcImage_t func)
{
    _func = func;
}

void ImageProcessor::process(const image_t &img)
{
    if(!img.data.empty())
        setImage(img);

    for(auto h : activeHandlers)
    {
        h->handle(_image);
    }
    _func(_image);
}

void ImageProcessor::setFilter(const QString &name, optionFilters *option)
{
    if(!option)
    {
        activeHandlers.clear();
        return;
    }
    for(auto h : handlers)
    {
        if(h->getName() == name)
        {
            activeHandlers.clear();
            activeHandlers.push_back(h);
            dynamic_cast<FilterHandler*>(h)->setOption(option);
        }
    }
}

// -----------------------------------------------------------------------------------
// ============================== Factory ============================================

IImageProcessor *FactoryImageProcessor::create()
{
    return new ImageProcessor();
}
