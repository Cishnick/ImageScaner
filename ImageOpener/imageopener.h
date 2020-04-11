#ifndef IMAGEOPENER_CLLASS_H
#define IMAGEOPENER_CLLASS_H

#include "iimageopener.h"

// Реализацию интерфейса объявляю в отдельном *.h файле, т.к. MOC не видит *.cpp
namespace _ImageOpener
{
    class ImageOpener : public IImageOpener
    {
        Q_OBJECT
    public:
        ImageOpener(QObject* parent) :
            IImageOpener (parent)
        {
            // Инициализируем фильтр из списка форматов
            Filter = "Image( ";
            for(auto format : _fileFormats)
            {
                Filter += '*' + format + ' ';
            }
            Filter += ")";
        }

        virtual ~ImageOpener() override = default;

    protected slots:
        virtual void openFile() override;

        virtual void getByteImage() override;

        virtual void getImage() override;

        virtual void openImage(const QImage &img, QString const& fileName) override;
    private:

        // Преобразует image в ByteImage, и инициализирует bImage
        void translateImage();

    private:
        // Поле, в котором хранится открытое изображение
        QImage image;

        // Поле, в котором хранится открытое изображение в виде массива байт
        ByteImage bImage;

        // Фильтр для окна открытия файла
        QString Filter;

        // Хранит имя последнего открытого файла
        QString _lastFileName;
\
    };
}

#endif // IMAGEOPENER_CLLASS_H
