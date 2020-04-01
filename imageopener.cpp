#include "imageopener.h"
#include <QFileDialog>

using namespace _ImageOpener;

// ----------------------------------- openFile ----------------------------------------
void ImageOpener::openFile()
{
    _lastFileName = QFileDialog::getOpenFileName(dynamic_cast<QWidget*>(parent()),
                                             "Открыть изображение",
                                             _dialogDir, Filter);
    if(!_lastFileName.isEmpty())
    {
        image = QImage(_lastFileName);
        translateImage();
        emit openedImage();
        emit openedImage(image);
        emit openedByteImage(bImage);
    }
}

// ----------------------------------- getByteImage ----------------------------------------
void ImageOpener::getByteImage()
{
    emit openedImage(image);
}

// ----------------------------------- getImage ----------------------------------------
void ImageOpener::getImage()
{
    emit openedByteImage(bImage);
}

// ----------------------------------- translateImage ----------------------------------------
void ImageOpener::translateImage()
{
    auto w = image.width();
    auto h = image.height();
    bImage.data = QByteArray(w*h, '\0');
    bImage.size = QSize(w, h);
    image = image.convertToFormat(QImage::Format::Format_Grayscale8);
    for(auto i = 0; i < w; i++)
    {
        for(auto j = 0; j < h; j++)
        {
            // Информацию укладываем по строкам
            bImage.data[i*h + j] = static_cast<char>(image.pixel(i, j));
        }
    }
}

// ----------------------------------- getLastFileName ----------------------------------------
void ImageOpener::getLastFileName()
{
    emit lastFileName(_lastFileName);
}

// ----------------------------------- openImage ----------------------------------------
void ImageOpener::openImage(const QImage &img)
{
    image = img;
    translateImage();
    emit openedImage();
    emit openedImage(image);
    emit openedByteImage(bImage);
}

// ----------------------------------- Factory::create ----------------------------------------
IImageOpener *FactoryImageOpener::create(QObject *parent)
{
    return new ImageOpener(parent);
}
