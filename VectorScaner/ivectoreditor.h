#ifndef IVECTOREDITOR_H
#define IVECTOREDITOR_H

#include <QObject>
#include "totaldefines.h"
#include "Parametres/iparametres.h"
#include "paramdata.h"

namespace _VectorScan
{
// ----------------------------- IVectorEditor --------------------------------------------
    class IVectorEditor : public QObject
    {
        Q_OBJECT
    protected:

        explicit IVectorEditor(QObject *parent): QObject(parent) {}

    signals:

        // Отправляется после установки первой точки, когда рисуется вектор
        void vectorPainted(Vector const& vect);

        // Отправляется при изменении параметров какого-либо вектора
        void vectorChanged(Vector const& vect);

        // Удаляет вектор с указанным цветом
        void vectorRemove(QColor color);

        // Отправляется после создания виджета
        void showWidget(QWidget* widget);

        // Испускается при закрытии окна
        void closed();

    public slots:

        // Слот для открытия изображения
        virtual void openedImage(QImage const& image, QString const& fname) = 0;
    };


// -------------------------------- FabricVectorEditor --------------------------------------
    class FactoryVectorEditor
    {
    public:
        static IVectorEditor* create(QObject* parent, IParametres* par);
    };
}
#endif // IVECTOREDITOR_H
