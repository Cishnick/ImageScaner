#ifndef CALCLINE_H
#define CALCLINE_H

#include <QObject>
#include "plotterwid.h"
#include "paramdata.h"
#include "iplotter.h"
namespace _VectorScan
{

    class Plotter : public IPlotter
    {
        Q_OBJECT
    public:
        explicit Plotter(QObject *parent, IParametres* par);

        virtual ~Plotter() override;
    signals:

        // Отображает виджет
        void show(QByteArray const&, QColor, int );

        // Перерисовывает график с новыми данными
        void redraw(QByteArray const&, int);

    private:

        QByteArray PostProcessing(QByteArray &&data);

        // Вовращает массив интерсивностей на векторе
        QByteArray BresenhamAlg();

    private:
        QPointer<PlotterWid> plot;

        image_t img;

        Vector vector;

        ParamPlotter *param = nullptr;

        // IPlotter interface
    public slots:
        virtual void vectorPainted(const Vector &vect) override;

        virtual void vectorChanged(const Vector &vect) override;

        virtual void openedByteImage(const image_t &image,
                                     QString const& filename) override;

        virtual void vectorRemove(QColor color) override;

        virtual QImage plotImage() override;

        virtual void updateParam(IParamData *) override;
    };
}

#endif // CALCLINE_H
