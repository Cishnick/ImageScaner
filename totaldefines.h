#ifndef TOTALDEFINES_H
#define TOTALDEFINES_H

#include <QObject>
#include <QImage>
#include <QByteArray>
#include <QDebug>
#include <QPointer>
#include <QAction>
#include <QFont>

/*
 * Данный файл содержит объявления, которые нужны почти во всех файлах.
 */

// ----------------ByteImage type ----------------------------------

struct ByteImage
{
    // Массив с пикселами изображения
    // Данные о пикселах укладываются по строкам!
    QByteArray  data;

    // Размеры изображения
    QSize       size;
};

// -----------------------------------------------------------------

// ------------------------- Vector type ---------------------------
struct Vector
{
    // Координата начала вектора
    QPointF begin;

    // Координата окончания вектора
    QPointF end;

    // Цвет вектора
    QColor color;
};

Q_DECLARE_METATYPE(Vector)
// -----------------------------------------------------------------

// ------------------------ EditMode -------------------------------

enum EditMode
{
    Editing,
    Drawing
};

// -----------------------------------------------------------------

#define DEBUG
// #define RELEASE


// ------------------- Директория, в которой открывается openFileDialog ----

const QString _dialogDir = "";
const QStringList _fileFormats = {"bmp", "tiff", "jpg", "jpeg", "png"};

// ---------------------------- Параметры -----------------------------------
struct ParamCalc
{
    int step; // Шаг по пикселам для графика
};

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

// Параметры графика
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



#endif // TOTALDEFINES_H
