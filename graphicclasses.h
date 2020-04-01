#ifndef GRAPHICCLASSES_H
#define GRAPHICCLASSES_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include "totaldefines.h"
#include <QEvent>
#include <algorithm>
#include <QHoverEvent>
#include <QGraphicsSceneHoverEvent>
#include <qmath.h>
namespace _VectorScan
{
// ------------------------------- VectorColor --------------------------------------------
// Этот класс нужен ля выбора случайного цвета
    struct VectorColor
    {
        // 27 цветов для 9
        // Кол-во цветов = (n / 3) ^ 3
        VectorColor(int n = 9) : N(n)
        {
            // Это надо чтобы в разных окнах были разные цвета
            static uint randNum = 15;
            qsrand(randNum++);

            numColors = (n / 3)*(n / 3)*(n / 3);
            auto i0 = 50;
            K = N / 3;
            d = (255 - i0) / K;
            QVector<int> Vr(K), Vg(K), Vb(K);
            int I = 0;
            std::generate(Vr.begin(), Vr.end(), [&I](){return I++; });
            std::random_shuffle(Vr.begin(), Vr.end());

            I = 0;
            std::generate(Vg.begin(), Vg.end(), [&I](){return I++; });
            std::random_shuffle(Vg.begin(), Vg.end());

            I = 0;
            std::generate(Vb.begin(), Vb.end(), [&I](){return I++; });
            std::random_shuffle(Vb.begin(), Vb.end());

            for(int i = 0; i < K; i++)
            {
                for(int j = 0; j < K; j++)
                {
                    for(int k = 0; k < K; k++)
                    {
                        colors.push_back(QColor(Vr[i]*d + d / 2,
                                                Vg[j]*d + d / 2,
                                                Vb[k]*d + d / 2 ));
                    }
                }
            }
        }

        void nextCurVal()
        {
            if(cur < numColors)
                cur++;
        }

        void prevCurVal()
        {
            if(cur > 0)
                cur--;
        }

        QColor getColor()
        {
            return colors.at(cur);
        }

    private:
        int numColors;
        int N;
        int K, d;
        int cur = 0;
        QVector<QColor>colors;
    };

// ------------------------------- VectorObject ------------------------------------------
    class VectorObject : public QGraphicsObject
    {
        Q_OBJECT
    public:
        VectorObject();
        virtual ~VectorObject() override;

        /*
         * Painting - вектор рисуется, отображение одно
         * Painted - вектор нарисован, отображение другое
         * Select - когда вектор выделен
         * Меняются либо Painting / Painted, либо Painted / Selected
         */
        enum ViewMode
        {
            Painting,
            Painted,
            Select
        };

    signals:
        // Вот только поэтому юзается QGraphicsObject вместо QGraphicsItem
        void vectorChanged(Vector const& vector);


        // QGraphicsItem interface
    public:
        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) override;

        // Задаем режим отображения вектора
        void setMode(ViewMode m);

        // Задаем вектор, меняя, по сути, ВСЕ.
        void setVector(Vector const& v);

        Vector const& getVector() const;

        void setEditMode(EditMode mode);

        bool isSelected();

        void setSelected(bool sel = false);

        // Останавливает редактирование объекта
        void stopEdit();

    private:
        // Все в этом элементе зависит от этого поля!
        Vector vector;
        Vector lastVector;  // Сохраняет предыдущее состояние вектора
        ViewMode viewMode;

        // Радиус круглешка в начале и конце вектора
        const int rad = 5;

        // Толщина линии этого круглешка
        const int circWid = 1;

        // Толщина линии самого вектора
        const int wid = 3;

        // Коэфициенты, определяющие увеличение размера контура относительно рисунка
        // Используются в shape(), и двух функциях ниже
        const qreal incR = 80, incW = 80;

        // Возвращает контур вокрут точек вектора и линии
        QPainterPath point1Border(bool exp = false) const;
        QPainterPath point2Border(bool exp = false) const;
        QPainterPath lineBorder(bool exp = false) const;

        // Зажата ли левая клавиша мыши
        bool leftPress = false;

        // Показывает последний элемент для редактирования
        // Это надо, чтобы, при перемещении начала вектора рядом с концом
        // не начинал перетаскиваться конец вектора. Ну, или наоборот.
        enum LastHandler
        {
            BeginPoint,
            EndPoint,
            Line,
            Nothing
        } lastHandler = Nothing;

        // В режиме рисования, вектор никак не реагирует на мышь.
        // Вернее, он не реагирует ни на что)
        EditMode eMode = EditMode::Drawing;

        // QGraphicsItem interface
    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

        // QGraphicsItem interface
    protected:
        virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

        virtual QPainterPath shape() const override;

        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    };

// ---------------------------------- VectorScene ---------------------------------------
    class VectorScene : public QGraphicsScene
    {
        Q_OBJECT
    public:
        VectorScene(QObject* parent = nullptr);
        virtual ~VectorScene() override = default;

        // Останавливает радектирование выделенного вектора
        void stopEdit();

        EditMode editMode();

        // Не меняет режим, если рисуется вектор!
        bool setEditMode(EditMode m);

    signals:
        void vectorPainted(Vector const& vector);

        void vectorChanged(Vector const& vector);

        void vectorRemove(QColor color);

        void setVectorStatus(int num, Vector const& selVector);

        // QGraphicsScene interface
    protected:

        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

        virtual void keyPressEvent(QKeyEvent *event) override;

    private:
        VectorColor vColor;
        EditMode    eMode = EditMode::Drawing;
        VectorObject* currentObj = nullptr;

        void removeVector();

    };

// -------------------------------- VectorView -------------------------------------------
    class VectorView : public QGraphicsView
    {
        Q_OBJECT
    public:
        VectorView(QWidget* parent = nullptr);
        virtual ~VectorView() override = default;

        void setImage(QImage const& img);

        // QGraphicsView interface
    protected:
        virtual void drawBackground(QPainter *painter, const QRectF &rect) override;

    private:
        QImage backImg;
    };

}

#endif // GRAPHICCLASSES_H
