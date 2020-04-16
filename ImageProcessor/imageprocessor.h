#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include "iimageprocessor.h"
#include "filterhandler.h"

class ImageProcessor : public IImageProcessor
{
public:
    ImageProcessor();

    virtual ~ImageProcessor();

    virtual void setImage(const image_t &) override;

    virtual const image_t &getImage() const override;

    virtual void setImageFunction(funcImage_t) override;

    virtual void process(const image_t &img) override;

    virtual void setFilter(const QString &name, optionFilters *option) override;


private:

    // Исходное изображение
    image_t _image;

    // Обработанное изображение
    image_t _resImage;

    // Функтор, куда выплевывается обработанное изображение
    funcImage_t _func;

    // Все обработчики
    std::list<IHandler*> handlers;

    // Активные обработчики
    std::list<IHandler*> activeHandlers;

};

#endif // IMAGEPROCESSOR_H
