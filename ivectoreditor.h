#ifndef IVECTOREDITOR_H
#define IVECTOREDITOR_H

#include <QObject>
#include "totaldefines.h"

namespace _VectorScan
{
// ----------------------------- IVectorEditor --------------------------------------------
    class IVectorEditor : public QObject
    {
        Q_OBJECT
    protected:
        // Да, это конструктор в абстрактном классе. Он нужен. Он неявно вызывается в
        // наследниках, чтобы задать parent QObject
        explicit IVectorEditor(QObject *parent = nullptr): QObject(parent) {}

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
        static IVectorEditor* create(QObject* parent = nullptr);
    };
}
#endif // IVECTOREDITOR_H
