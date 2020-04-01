#ifndef PLOTTERWIDSTRUCTS_H
#define PLOTTERWIDSTRUCTS_H

// Здесь собраны структуры, необходимые для PlotterWid
#include "totaldefines.h"
#include <QFont>
#include <QAction>

// Поля. Значения полей в пикселах
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
    int widthStep;
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

struct Param
{
    Margin margin;
    Axe xAxe;
    Axe yAxe;
    Grid xGrid;
    Grid yGrid;
    QRect rectGraph; // Прямоугольник, в котором рисуется график
    qreal plotWidth;
    Qt::PenStyle plotPenStyle;
};

#endif // PLOTTERWIDSTRUCTS_H