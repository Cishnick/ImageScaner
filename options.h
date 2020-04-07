#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>
#include <QLayout>
#include "totaldefines.h"


namespace Ui {
class Options;
}

class Options : public QWidget
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = nullptr);
    ~Options();

    void setBeginCalcParam(ParamCalc const& param);

    void setBeginPlotParam(Param const& param);

signals:
    void setCalcParam(ParamCalc const&);

    void setPlotParam(Param const&);

private slots:
    void on_spinWidthPlot_valueChanged(double arg1);

    void on_sliderGridY_valueChanged(int value);

    void on_spinGridY_valueChanged(int arg1);

    void on_spinStep_valueChanged(int arg1);

private:

    ParamCalc paramCalc;
    Param     paramPlot;

    Ui::Options *ui;
};

#endif // OPTIONS_H
