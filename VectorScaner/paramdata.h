#ifndef CONCRETEPARAMDATA_H
#define CONCRETEPARAMDATA_H
#include "Parametres/iparamdata.h"
#include <QColor>
#include <QDebug>
#include <QFont>
#include <QRect>

// ================================ ParamDataPlotter =====================================
class ParamPlotter : public IParamData
{
public:
    ParamPlotter(QSettings* settings) : IParamData(settings)
    {
        auto temp = settings->value(_key + "step", 1000).toInt();
        if(temp != 1000)
        {
            _step = temp;
        }
    }

    virtual void saveData() override
    {
        settings->setValue(_key + "step", _step);
    }

    // --------------------------- getters -------------------------------
    int step() const
    {
        return _step;
    }

protected:
    int _step = 1;
    const QString _key = "plotter/";
};

// ======================== ParamVectorColor ====================================
class ParamVectorColor : public IParamData
{
public:
    ParamVectorColor(QSettings* settings) : IParamData(settings)
    {
        auto temp = settings->value(_key + "number", 0).toInt();
        if(temp != 0)
        {
            _number = temp;
            _saturation = settings->value(_key + "saturation").toInt();
            _velocity = settings->value(_key + "velocity").toInt();
        }
        else
        {
            saveData();
        }
    }

    virtual void saveData() override
    {
        settings->setValue(_key + "number", _number);
        settings->setValue(_key + "saturation", _saturation);
        settings->setValue(_key + "velocity", _velocity);
    }
    // ------------------------ getters --------------------------------

    int number() const
    {
        return _number;
    }

    int saturation() const
    {
        return _saturation;
    }

    int velocity() const
    {
        return _velocity;
    }

protected:
    int _number = 10;
    int _saturation = 255;
    int _velocity = 127;
    const QString _key = "vector_color/";
};

// ============================== ParamVectorObject ===============================

class ParamVectorObject : public IParamData
{
public:
    ParamVectorObject(QSettings* settings) : IParamData(settings)
    {
        auto temp = settings->value(_key + "rad", 0).toInt();
        if(temp != 0)
        {
            _rad                = temp;
            _circWid            = settings->value(_key +"circWid").toInt();
            _lineWid            = settings->value(_key +"lineWid").toInt();
            _selColorBack       = settings->value(_key +"selColorBack").value<QColor>();
            _colorBack          = settings->value(_key +"colorBack").value<QColor>();
            _colorBeginPoint    = settings->value(_key +"colorBeginPoint").value<QColor>();
            _colorEndPoint      = settings->value(_key +"colorEndPoint").value<QColor>();
            _styleBack          = static_cast<Qt::PenStyle>(settings->value(_key +"styleBack").toUInt());
            _styleFront         = static_cast<Qt::PenStyle>(settings->value(_key +"styleFront").toUInt());
            _incR               = settings->value(_key +"incR").toReal();
            _incW               = settings->value(_key +"incW").toReal();
        }
        else
        {
            saveData();
        }
    }

    virtual void saveData() override
    {
        settings->setValue(_key + "rad", _rad);
        settings->setValue(_key + "circWid", _circWid);
        settings->setValue(_key + "lineWid", _lineWid);
        settings->setValue(_key + "selColorBack", _selColorBack);
        settings->setValue(_key + "colorBack", _colorBack);
        settings->setValue(_key + "colorBeginPoint", _colorBeginPoint);
        settings->setValue(_key + "colorEndPoint", _colorEndPoint);
        settings->setValue(_key + "styleBack",  static_cast<uint>(_styleBack));
        settings->setValue(_key + "styleFront", static_cast<uint>(_styleFront));
        settings->setValue(_key + "incR", _incR);
        settings->setValue(_key + "incW", _incW);
    }

// ---------------------------- getters ---------------------------------------

    int rad() const
    {
        return _rad;
    }

    int circWid() const
    {
        return _circWid;
    }

    int lineWid() const
    {
        return _lineWid;
    }

    QColor selColorBack() const
    {
        return _selColorBack;
    }

    QColor colorBack() const
    {
        return _colorBack;
    }

    QColor colorBeginPoint() const
    {
        return _colorBeginPoint;
    }

    QColor colorEndPoint() const
    {
        return _colorEndPoint;
    }

    Qt::PenStyle styleBack() const
    {
        return _styleBack;
    }

    Qt::PenStyle styleFront() const
    {
        return _styleFront;
    }

    qreal incR() const
    {
        return _incR;
    }

    qreal incW() const
    {
        return _incW;
    }

protected:

    int             _rad = 5;                        // Радиус точки в начале и конце вектора
    int             _circWid = 1;                    // Ширина линии, которой рисуется контур этой точки
    int             _lineWid = 3;                    // Ширина линии, которой рисуется сам вектор
    QColor          _selColorBack = Qt::white;       // Цвет фона выбранного вектора
    QColor          _colorBack = Qt::gray;           // Цвет фона невыбранного вектора
    Qt::PenStyle    _styleBack = Qt::SolidLine;      // Стиль линии фона
    Qt::PenStyle    _styleFront = Qt::DashLine;      // Стиль линии рисующейся на фоне
    QColor          _colorBeginPoint = Qt::gray;     // Цвет начальной точки вектора
    QColor          _colorEndPoint = Qt::white;      // Цвет конечной точки вектора
    qreal           _incR = 50;                      // Увеличение rad во время перетаскивания
    qreal           _incW = 80;                      // Увеличение lineWid во время перетаскивания
    const QString   _key = "vector_object/";
};

// ============================== ParamVectorScan ===============================
class ParamVectorScan : public IParamData
{
public:
    ParamVectorScan(QSettings* settings) : IParamData(settings)
    {
        auto temp = settings->value(_key + "step", "\n\n\n").toString();
        if(temp != "\n\n\n")
        {
            _saveFilePath = temp;
        }
        else
        {
            saveData();
        }
    }

    virtual void saveData() override
    {
        settings->setValue(_key + "step", _saveFilePath);
    }

    // --------------------------- getters -------------------------------
    QString saveFilePath() const
    {
        return _saveFilePath;
    }

protected:
    QString _saveFilePath = "";
    const QString _key = "vector_scan/";
};

// ============================ ParamPlotterWid ============================
class ParamPlotterWid : public IParamData
{
public:
    // Поля
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
        qreal           width;      // Ширина линии оси в пикселах
        QColor          color;      // Цвет линии
        Qt::PenStyle    penStyle;   // Стиль пера для рисования оси
        QString         label;      // Текст для оси
    };

    struct Grid
    {
        int beginVal;           // Начальное значение оси
        int endVal;             // Конечноезначение оси
        int steps;              // Количество шагов сетки
        QColor colorLine;       // Цвет линии сетки
        QColor colorText;       // Цвет текста подписей
        QFont textFont;         // Шрифт текста подписей
        Qt::PenStyle penStyle;  // Стиль линии сетки
        qreal widthLine;        // Ширина линии сетки
    };
    enum YValueMode
    {
        Percent,        // Проценты
        Intensity       // Интенсивность (0..255)
    };
    ParamPlotterWid(QSettings* settings) : IParamData(settings)
    {
        auto temp = settings->value(_key + "plotWidth", -1.).toReal();
        if(temp > 0.)
        {
            _plotWidth          = temp;
            _margin.left        = settings->value(_key + "margin_left").toInt();
            _margin.right       = settings->value(_key + "margin_right").toInt();
            _margin.top         = settings->value(_key + "margin_top").toInt();
            _margin.bottom      = settings->value(_key + "margin_bottom").toInt();
            _xAxe.color         = settings->value(_key + "xAxe_color").value<QColor>();
            _xAxe.label         = settings->value(_key + "xAxe_label").toString();
            _xAxe.width         = settings->value(_key + "xAxe_width").toReal();
            _xAxe.penStyle      = static_cast<Qt::PenStyle>(settings->value(_key + "xAxe_penStyle").toUInt());
            _yAxe.color         = settings->value(_key + "yAxe_color").value<QColor>();
            _yAxe.label         = settings->value(_key + "yAxe_label").toString();
            _yAxe.width         = settings->value(_key + "yAxe_width").toReal();
            _yAxe.penStyle      = static_cast<Qt::PenStyle>(settings->value(_key + "yAxe_penStyle").toUInt());

            _xGrid.steps        = settings->value(_key + "xGrid_steps").toInt();
            _xGrid.endVal       = settings->value(_key + "xGrid_endVal").toInt();
            _xGrid.beginVal     = settings->value(_key + "xGrid_beginVal").toInt();
            _xGrid.penStyle     = static_cast<Qt::PenStyle>(settings->value(_key + "xGrid_penStyle").toInt());
            _xGrid.textFont     = settings->value(_key + "xGrid_textFont").value<QFont>();
            _xGrid.colorLine    = settings->value(_key + "xGrid_colorLine").value<QColor>();
            _xGrid.colorText    = settings->value(_key + "xGrid_colorText").value<QColor>();
            _xGrid.widthLine    = settings->value(_key + "xGrid_widthLine").toInt();

            _yGrid.steps        = settings->value(_key + "yGrid_steps").toInt();
            _yGrid.endVal       = settings->value(_key + "yGrid_endVal").toInt();
            _yGrid.beginVal     = settings->value(_key + "yGrid_beginVal").toInt();
            _yGrid.penStyle     = static_cast<Qt::PenStyle>(settings->value(_key + "yGrid_penStyle").toInt());
            _yGrid.textFont     = settings->value(_key + "yGrid_textFont").value<QFont>();
            _yGrid.colorLine    = settings->value(_key + "yGrid_colorLine").value<QColor>();
            _yGrid.colorText    = settings->value(_key + "yGrid_colorText").value<QColor>();
            _yGrid.widthLine    = settings->value(_key + "yGrid_widthLine").toInt();

            _plotPenStyle       = static_cast<Qt::PenStyle>(settings->value(_key + "plotPenStyle").toUInt());
            _yMode              = static_cast<YValueMode>(settings->value(_key + "yMode").toUInt());
        }
        else
        {
            _xAxe.color    = Qt::GlobalColor::black;
            _xAxe.label    = "X";
            _xAxe.width    = 1;
            _xAxe.penStyle = Qt::PenStyle::SolidLine;

            _yAxe.color     = Qt::GlobalColor::black;
            _yAxe.label     = "Y";
            _yAxe.width     = 1;
            _yAxe.penStyle  = Qt::PenStyle::SolidLine;

            _xGrid.beginVal  = 0;
            _xGrid.endVal    = 100;
            _xGrid.steps     = 1; // Не выставлять в 0! Иначе не скомпилится
            _xGrid.penStyle  = Qt::PenStyle::DashLine;
            _xGrid.colorLine = Qt::GlobalColor::gray;
            _xGrid.widthLine = 1;
            _xGrid.colorText = Qt::GlobalColor::black;
            _xGrid.textFont  = QFont("Times New Roman", 10);

            _yGrid.beginVal  = 0;
            _yGrid.endVal    = 255;
            _yGrid.steps     = 10;
            _yGrid.penStyle  = Qt::PenStyle::DashDotLine;
            _yGrid.colorLine = Qt::GlobalColor::darkGray;
            _yGrid.widthLine = 1;
            _yGrid.colorText = Qt::GlobalColor::black;
            _yGrid.textFont  = QFont("Times New Roman", 10);

            _margin.left   = 30;
            _margin.right  = 30;
            _margin.top    = 30;
            _margin.bottom = 30;

            _plotWidth = 1.6;
            _plotPenStyle = Qt::PenStyle::SolidLine;
            _yMode = ParamPlotterWid::Percent;
            saveData();
        }
    }

    virtual void saveData() override
    {
        settings->setValue(_key + "plotWidth", _plotWidth);
        settings->setValue(_key + "margin_left", _margin.left);
        settings->setValue(_key + "margin_right", _margin.right);
        settings->setValue(_key + "margin_top", _margin.top);
        settings->setValue(_key + "margin_bottom", _margin.bottom);
        settings->setValue(_key + "xAxe_color", _xAxe.color);
        settings->setValue(_key + "xAxe_label", _xAxe.label);
        settings->setValue(_key + "xAxe_width", _xAxe.width);
        settings->setValue(_key + "xAxe_penStyle", static_cast<uint>(_xAxe.penStyle));
        settings->setValue(_key + "yAxe_color", _yAxe.color);
        settings->setValue(_key + "yAxe_label", _yAxe.label);
        settings->setValue(_key + "yAxe_width", _yAxe.width);
        settings->setValue(_key + "yAxe_penStyle", static_cast<uint>(_yAxe.penStyle));
        settings->setValue(_key + "xGrid_steps", _xGrid.steps);
        settings->setValue(_key + "xGrid_endVal", _xGrid.endVal);
        settings->setValue(_key + "xGrid_beginVal", _xGrid.beginVal);
        settings->setValue(_key + "xGrid_penStyle",  static_cast<uint>(_xGrid.penStyle));
        settings->setValue(_key + "xGrid_textFont", _xGrid.textFont);
        settings->setValue(_key + "xGrid_colorLine", _xGrid.colorLine);
        settings->setValue(_key + "xGrid_colorText", _xGrid.colorText);
        settings->setValue(_key + "xGrid_widthLine", _xGrid.widthLine);
        settings->setValue(_key + "yGrid_steps", _yGrid.steps);
        settings->setValue(_key + "yGrid_endVal", _yGrid.endVal);
        settings->setValue(_key + "yGrid_beginVal", _yGrid.beginVal);
        settings->setValue(_key + "yGrid_penStyle",  static_cast<uint>(_yGrid.penStyle));
        settings->setValue(_key + "yGrid_textFont", _yGrid.textFont);
        settings->setValue(_key + "yGrid_colorLine", _yGrid.colorLine);
        settings->setValue(_key + "yGrid_colorText", _yGrid.colorText);
        settings->setValue(_key + "yGrid_widthLine", _yGrid.widthLine);
        settings->setValue(_key + "plotPenStyle", static_cast<uint>(_plotPenStyle));
        settings->setValue(_key + "yMode",  static_cast<uint>(_yMode));
    }

    // --------------------------------------- getters ------------------------

    Margin margin() const
    {
        return _margin;
    }
    Axe xAxe() const
    {
        return _xAxe;
    }
    Axe yAxe() const
    {
        return _yAxe;
    }
    const Grid& xGrid() const
    {
        return _xGrid;
    }
    Grid yGrid() const
    {
        return _yGrid;
    }
    qreal plotWidth() const
    {
        return _plotWidth;
    }
    Qt::PenStyle plotPenStyle() const
    {
        return _plotPenStyle;
    }
    YValueMode yMode() const
    {
        return _yMode;
    }
protected:

    Margin          _margin;        // Поля для прямоугольника с графиком
    Axe             _xAxe;          // х ось
    Axe             _yAxe;          // у ось
    Grid            _xGrid;         // Сетка по х
    Grid            _yGrid;         // Сетка по у
    qreal           _plotWidth;     // Толщина линиии графика
    Qt::PenStyle    _plotPenStyle;  // Стиль пера для рисования графика
    YValueMode      _yMode;         // Режим отображения данных по оси У
    const QString   _key = "plotter_widget/";
};

#endif // CONCRETEPARAMDATA_H
