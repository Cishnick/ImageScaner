#ifndef IPLOTTER_H
#define IPLOTTER_H

#include <QObject>
#include "totaldefines.h"
#include "Parametres/iparamdata.h"
#include "paramdata.h"
#include "Parametres/iparametres.h"

namespace _VectorScan
{
    class IPlotter : public QObject
    {
        Q_OBJECT
    public:
        explicit IPlotter(QObject *parent = nullptr) : QObject(parent) {}

        virtual QImage plotImage() = 0;

    signals:

        // Отправляется после создания виджета
        void showWidget(QWidget* widget);

        // Испускается при закрытии окна с графиком
        void closed();


    public slots:


        // При создании вектора. Сначала надо послать этот сигнал, потом следующий
        virtual void vectorPainted(Vector const& vect) = 0;

        // Изменение вектора
        virtual void vectorChanged(Vector const& vect) = 0;

        // Удаляет вектор с указанным цветом
        virtual void vectorRemove(QColor color) = 0;

        // Открытие изображения
        virtual void openedByteImage(image_t const& image,
                                     QString const& filename) = 0;

        virtual void updateParam(IParamData*) = 0;
    };

    class FactoryPlotter
    {
    public:
        static IPlotter* create(QObject* parent, IParametres* par);
    };
}
#endif // IPLOTTER_H
