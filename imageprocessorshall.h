#ifndef IMAGEPROCESSORSHALL_H
#define IMAGEPROCESSORSHALL_H
#include <QObject>
#include "totaldefines.h"
#include "ImageProcessor/iimageprocessor.h"

class ImageProcessorShall : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessorShall(QObject *parent = nullptr);

    const QImage& getImage();

signals:

    void processed(QImage const&, QString const&);

    void byteImage(image_t const&, QString const&);

    void setStatusText(QString const&);

public slots:

    void setData(QImage const& img, QString const&);

    void process();

    void setFilter(QString const& name, optionFilters* option);

    void setSaveImage(bool save);

private:

    image_t QImageToImage_t(QImage const& img);

    QImage Image_tToQImage(image_t const& img);

    IImageProcessor* processor;

    QString filename;

    QImage img;

    std::thread thread;

    bool saveImage;
};

#endif // IMAGEPROCESSORSHALL_H
