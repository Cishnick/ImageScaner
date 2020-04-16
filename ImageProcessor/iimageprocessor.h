#ifndef IIMAGEPROCESSOR_H
#define IIMAGEPROCESSOR_H
#include "defines.h"

class IImageProcessor
{
public:

    // Задает изображение
    virtual void setImage(image_t const&) = 0;

    // Возвращает заданное изображение
    virtual image_t const& getImage() const = 0;

    // Задает функтор, в который будет скидываться итоговое или промежуточное
    // изображение
    virtual void setImageFunction(funcImage_t) = 0;

    // Запускает обработку изображения
    virtual void process(image_t const& img = image_t()) = 0;

    // Задает фильтр с параметрами
    virtual void setFilter(QString const& name, optionFilters* option) = 0;
};

class FactoryImageProcessor
{
public:
    static IImageProcessor* create();
};


#endif // IIMAGEPROCESSOR_H
