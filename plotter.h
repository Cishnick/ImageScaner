#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include "totaldefines.h"
#include "iplotter.h"

namespace _VectorScan
{

// Интерфейс для взаимодействия с остальными модулями программы
    class IPlotterWid : public QWidget
    {
        Q_OBJECT
    public:
        IPlotterWid(QWidget* parent = nullptr)
            : QWidget(parent)
        { }

        virtual QImage plotImage() = 0;

    public slots:

        virtual void show(const QByteArray &, QColor color) = 0;

        virtual void redraw(const QByteArray &, QColor color) = 0;

        // Удаляет график с указанным цветом
        virtual void removePlot(QColor color) = 0;

    signals:

        // Сигнал для отображения виджета с графиком
        void showWidget(QWidget*);

        void closed();

    };

    class FactoryPlotterWid
    {
    public:
        static IPlotterWid* create(QWidget* parent = nullptr);
    };
}

#endif // PLOTTER_H
