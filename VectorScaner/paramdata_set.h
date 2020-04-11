#ifndef PARAMWIDGETVECTORSCAN_H
#define PARAMWIDGETVECTORSCAN_H
#include "Parametres/iparamwidget.h"
#include "paramdata.h"

class ParamPlotter_set : public ParamPlotter
{
public:
    ParamPlotter_set(QSettings* settings) : ParamPlotter(settings) {}

    void setStep(int step)
    {
        _step = step;
    }
};

class ParamVectorColor_set : public ParamVectorColor
{
public:
    ParamVectorColor_set(QSettings* settings) : ParamVectorColor(settings) {}

    void setNumber(int number)
    {
        _number = number;
    }
    void setSaturation(int saturation)
    {
        _saturation = saturation;
    }
    void setVelocity(int velocity)
    {
        _velocity = velocity;
    }
};

class ParamVectorObject_set : public ParamVectorObject
{
public:
    ParamVectorObject_set(QSettings* settings) : ParamVectorObject(settings) {}

    void setRad(int rad)
    {
        _rad = rad;
    }
    void setCircWid(int circWid)
    {
        _circWid = circWid;
    }
    void setLineWid(int lineWid)
    {
        _lineWid = lineWid;
    }
    void setSelColorBack(QColor selColorBack)
    {
        _selColorBack = selColorBack;
    }
    void setColorBack(QColor colorBack)
    {
        _colorBack = colorBack;
    }
    void setStyleBack(Qt::PenStyle styleBack)
    {
        _styleBack = styleBack;
    }
    void setStyleFront(Qt::PenStyle styleFront)
    {
        _styleBack = styleFront;
    }
    void setColorBeginPoint(QColor color)
    {
        _colorBeginPoint = color;
    }
    void setColorEndPoint(QColor color)
    {
        _colorEndPoint = color;
    }
    void setIncR(qreal incR)
    {
        _incR = incR;
    }
    void setIncW(qreal incW)
    {
        _incW = incW;
    }
};

class ParamVectorScan_set : public ParamVectorScan
{
public:
    ParamVectorScan_set(QSettings* settings) : ParamVectorScan(settings) {}

    void setSaveFilePath(QString const& saveFilePath)
    {
        _saveFilePath = saveFilePath;
    }
};

class ParamPlotterWid_set : public ParamPlotterWid
{
public:
    ParamPlotterWid_set(QSettings* settings) : ParamPlotterWid(settings) {}

    void setMargin(Margin margin)
    {
        _margin = margin;
    }
    void setXAxe(Axe x)
    {
        _xAxe = x;
    }
    void setYAxe(Axe y)
    {
        _yAxe = y;
    }
    void setXGrid(Grid x)
    {
        _xGrid = x;
    }
    void setYGrid(Grid y)
    {
        _yGrid = y;
    }
    void setPlotWidth(qreal wid)
    {
        _plotWidth = wid;
    }
    void setPlotPenStyle(Qt::PenStyle style)
    {
        _plotPenStyle = style;
    }
    void setYMode(YValueMode mode)
    {
        _yMode = mode;
    }
};
#endif // PARAMWIDGETVECTORSCAN_H
