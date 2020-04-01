#ifndef CALCLINE_H
#define CALCLINE_H

#include <QObject>
#include "plotter.h"

namespace _VectorScan

{
    class CalcLine : public IPlotter
    {
        Q_OBJECT
    public:
        explicit CalcLine(QObject *parent = nullptr);

        virtual ~CalcLine() override;


    signals:

        // Отображает виджет
        void show(QByteArray const&, QColor);

        // Перерисовывает график с новыми данными
        void redraw(QByteArray const&, QColor);
    private:

        // Вовращает массив интерсивностей на векторе
        QByteArray BresenhamAlg();

    private:
        QPointer<IPlotterWid> plot;

        ByteImage img;

        Vector vector;

        int step = 1;

        // IPlotter interface
    public slots:
        virtual void vectorPainted(const Vector &vect) override;

        virtual void vectorChanged(const Vector &vect) override;

        virtual void openedByteImage(const ByteImage &image,
                                     QString const& filename) override;

        virtual void vectorRemove(QColor color) override;

        virtual QImage plotImage() override;
    };
}

#endif // CALCLINE_H
