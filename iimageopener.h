#ifndef IMAGEOPENER_H
#define IMAGEOPENER_H

#include <QObject>
#include "totaldefines.h"

namespace _ImageOpener
{
    class IImageOpener : public QObject
    {
        Q_OBJECT
    protected:
        explicit IImageOpener(QObject *parent = nullptr) : QObject(parent) {}
        virtual ~IImageOpener() = default;
    signals:

        // Передает открытое изображение
        void openedImage(QImage const&, QString const);

        // Передает открытое изображение в виде массива байт
        void openedByteImage(ByteImage const&, QString const&);

        // Испускается после открытия изображения
        void openedImage();

    public slots:

        // Команда открыть файл
        virtual void openFile() = 0;

        // Делает то же, что и при openFile, но уже с готовым изображением
        virtual void openImage(QImage const& img, QString const& fileName) = 0;

        // Инициирует отправку сигнала openedByteImage
        virtual void getByteImage() = 0;

        // Инициирует отправку сигнала openedByteImage
        virtual void getImage() = 0;
    };

    // Создает реализацию IImageOpener
    class FactoryImageOpener
    {
    public:
        static IImageOpener* create(QObject* parent = nullptr);
    };

}
#endif // IMAGEOPENER_H
