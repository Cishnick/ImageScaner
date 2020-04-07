#ifndef CALCLINE_H
#define CALCLINE_H

#include <QObject>
#include "plotterwid.h"
#include "options.h"
#include "iplotter.h"
namespace _VectorScan
{

    class Plotter : public IPlotter
    {
        Q_OBJECT
    public:
        explicit Plotter(QObject *parent = nullptr);

        virtual ~Plotter() override;
    signals:

        // Отображает виджет
        void show(QByteArray const&, QColor);

        // Перерисовывает график с новыми данными
        void redraw(QByteArray const&, QColor);

    private:

        QByteArray PostProcessing(QByteArray &&data);

        // Вовращает массив интерсивностей на векторе
        QByteArray BresenhamAlg();

    private:
        QPointer<PlotterWid> plot;

        ByteImage img;

        Vector vector;

        ParamCalc param;

        QPointer<Options> windOpt;

        // IPlotter interface
    public slots:
        virtual void vectorPainted(const Vector &vect) override;

        virtual void vectorChanged(const Vector &vect) override;

        virtual void openedByteImage(const ByteImage &image,
                                     QString const& filename) override;

        virtual void vectorRemove(QColor color) override;

        virtual QImage plotImage() override;

        virtual void openWindowOption() override;

    private slots:

        void setParam(ParamCalc const& param);
    };
}

#endif // CALCLINE_H
