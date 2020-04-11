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
        emit openedImage(image, _lastFileName);
        emit openedByteImage(bImage, _lastFileName);
    }
}

// ----------------------------------- getByteImage ----------------------------------------
void ImageOpener::getByteImage()
{
    emit openedImage(image, _lastFileName);
}

// ----------------------------------- getImage ----------------------------------------
void ImageOpener::getImage()
{
    emit openedByteImage(bImage, _lastFileName);
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

// ----------------------------------- openImage ----------------------------------------
void ImageOpener::openImage(const QImage &img, QString const& fileName)
{
    image = img;
    _lastFileName = fileName;
    translateImage();
    emit openedImage();
    emit openedImage(image, fileName);
    emit openedByteImage(bImage, fileName);
}

// ----------------------------------- Factory::create ----------------------------------------
IImageOpener *FactoryImageOpener::create(QObject *parent)
{
    return new ImageOpener(parent);
}
