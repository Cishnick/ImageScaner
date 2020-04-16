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

typedef struct image_t
{
    // Данные в массиве хранятся построчно!
    // Формат индекса для доступа
    std::vector<uint8_t> data;
    size_t width;
    size_t height;
}image_t;

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

    // Идентификатор вектора
    int id;
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
QSize const _ParamWidgetSize = QSize(480, 360);


#endif // TOTALDEFINES_H
