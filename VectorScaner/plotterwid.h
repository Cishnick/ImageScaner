#ifndef PLOTTERWID_H
#define PLOTTERWID_H
#include <QPainter>
#include <QPaintEvent>
#include <QPaintDevice>
#include <QContextMenuEvent>
#include <QMenu>
#include "totaldefines.h"
#include <tgmath.h>
#include "paramdata.h"
#include "Parametres/iparametres.h"
namespace _VectorScan
{
    // Виджет, для рисования графика
    class PlotterWid : public QWidget
    {
        Q_OBJECT

        struct Plot
        {
            QVector<QPointF> points;
            qreal width;
            QColor color;
            QAction* act;
            int id;
            bool isVisible;
        };

    public:
        explicit PlotterWid(QWidget *parent, IParametres* par);

    public slots:
        void show(const QByteArray &, QColor color, int id);

        void redraw(const QByteArray &, int id);

        void removePlot(QColor color);

        QImage plotImage();

    signals:
        // Сигнал для отображения виджета с графиком
        void showWidget(QWidget*);

        void closed();

    public slots:
        virtual void paintEvent(QPaintEvent* event) override;

        void updateParam(IParamData *);
    private:
        ParamPlotterWid *param = nullptr; // Парам парам :D(Раньше ParamPlot назывался просто Param) Параметры всего в этом классе

        QRect rectGraph;

        QByteArrayList dataList; // Данные для графика. Хранить их надо для пересчета координат при ресайзе
        QList<Plot>  plotList; // Графики

        // Показывает, отображается ли на экране виджет
        // Нужен, чтобы не отображать новое окно при рисовании нового графика
        bool isShow = false;

        // Это конткестная менюшка для скрытия ненужных графиков
        QMenu* cMenu;

        QAction* actHideAll, *actShowAll;

        void drawGrid(QPainter& painter);

        // Данные в plot
        void drawGraph(QPainter& painter);

        // Пересчитывает данные для графика в точки и сохраняет в plot
        // i - индекс графика.
        void recalc(QByteArray const& data, int i);

        // Пересчитывает все графики
        void recalcAll();

    private slots:
        void triggeredContext(QAction* act);

        // QWidget interface
    protected:
        virtual void resizeEvent(QResizeEvent *event) override;

        virtual void closeEvent(QCloseEvent *event) override;

        virtual void changeEvent(QEvent * event) override;

        virtual void contextMenuEvent(QContextMenuEvent *event) override;

    };


}
#endif // PLOTTERWID_H
