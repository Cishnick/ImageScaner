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

#endif // TOTALDEFINES_H
