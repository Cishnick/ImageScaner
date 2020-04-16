#include "imageprocessorshall.h"

ImageProcessorShall::ImageProcessorShall(QObject *parent) : QObject(parent)
{
    processor = FactoryImageProcessor::create();

    processor->setImageFunction([this](image_t const& data){
        auto temp = Image_tToQImage(data);
        if(saveImage)
        {
            temp.save("Filtered_"+filename, ".bmp");
        }
        emit this->processed(temp, filename);
        emit this->byteImage(data, filename);
        thread.detach();
    });
}

const QImage &ImageProcessorShall::getImage()
{
    return std::move(Image_tToQImage(processor->getImage()));
}

void ImageProcessorShall::setData(const QImage &img, const QString &filename)
{
    this->filename = filename;
    this->img = img;
}

void ImageProcessorShall::process()
{
    if(!img.isNull())
    {
        thread = std::thread([this]()
        {
            processor->process(QImageToImage_t(img));
        });
    }
    else
    {
        emit setStatusText("Готов");
    }
}

void ImageProcessorShall::setFilter(const QString &name, optionFilters *option)
{
    processor->setFilter(name, option);
}

void ImageProcessorShall::setSaveImage(bool save)
{
    saveImage = save;
}

image_t ImageProcessorShall::QImageToImage_t(const QImage &img)
{
    image_t resImg;
    resImg.width  = static_cast<size_t>(img.bytesPerLine());
    resImg.height = static_cast<size_t>(img.height());
    resImg.data.resize(resImg.width*resImg.height);
    int C = img.bytesPerLine() / img.width();
    auto itV = resImg.data.begin();
    if(C == 4)
    {
        resImg.width  = static_cast<size_t>(img.width());
        resImg.data.resize(resImg.width*resImg.height);
        auto itImg = reinterpret_cast<const uint32_t*>(img.bits());
        for(; itV < resImg.data.end(); itV++, itImg++)
        {
            *itV = static_cast<uint8_t>(qGreen(*itImg));
        }
    }
    else if(C == 1)
    {
//        if(img.bytesPerLine() % img.width())
//        {
//            resImg.width = img.bytesPerLine();
//            resImg.data.resize(resImg.width*resImg.height);
//        }
        auto itImg = reinterpret_cast<const uint8_t*>(img.bits());
        for(; itV < resImg.data.end(); itV++, itImg++)
        {
            *itV = static_cast<uint8_t>(qGreen(img.color(*itImg)));
        }
    }
    resImg.data.shrink_to_fit();
    return resImg;
}

QImage ImageProcessorShall::Image_tToQImage(const image_t &img)
{
    QImage resImg(QSize(static_cast<int>(img.width), static_cast<int>(img.height)),
                  QImage::Format_RGB32);
    auto itImg = reinterpret_cast<uint32_t*>(resImg.bits());
    auto itV = img.data.begin();
    for(; itV < img.data.end(); itV++, itImg++)
    {
        *itImg = qRgb(0, *itV, 0);
    }

    return resImg;
}
