#ifndef PARAMETRES_H
#define PARAMETRES_H
/*
 * В данном файле собраны все параметры программы
 *
 */
#include <QRect>
#include <QColor>
#include <QFont>
#include <QAction>

namespace _VectorScan
{


    // Поля области рисования графика пикселей. Значения полей в пикселах
    struct Margin
    {
        int left;
        int right;
        int top;
        int bottom;
    };

    // Эта структура описывает ось
    struct Axe
    {
        qreal width;    // Ширина линии оси в пикселах
        bool isArrow;   // Есть ли у оси стрелка
        QColor color;   // Цвет линии
        Qt::PenStyle penStyle; // Стиль пера для рисования оси
        QString label;  // Текст для оси
    };

    struct Grid
    {
        int beginVal;
        int endVal;
        int steps;
        QColor colorLine;
        QColor colorText;
        QFont textFont;
        Qt::PenStyle penStyle;
        qreal widthLine;
    };

    struct Plot
    {
        QVector<QPointF> points;
        qreal width;
        QColor color;
        Qt::PenStyle penStyle;
        QAction* act;
        bool isVisible;
    };

    // Параметры графика
    struct ParamPlot
    {
        Margin margin;
        Axe xAxe;
        Axe yAxe;
        Grid xGrid;
        Grid yGrid;
        QRect rectGraph; // Прямоугольник, в котором рисуется график
        qreal plotWidth;
        Qt::PenStyle plotPenStyle;
        enum YValueMode
        {
            Percent,
            Intensity
        } yMode = Percent;
    };


    struct ParamCalc
    {
        int step; // Шаг по пикселам для графика
    };

}



#endif // PARAMETRES_H
