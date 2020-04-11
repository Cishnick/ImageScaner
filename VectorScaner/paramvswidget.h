#ifndef PARAMVSWIDGET_H
#define PARAMVSWIDGET_H

#include "Parametres/iparamwidget.h"
#include "paramdata_set.h"
#include <QDialog>
#include <QColorDialog>
namespace Ui {
class ParamVSWidget;
}

class ParamVSWidget : public IParamWidget
{
    Q_OBJECT

public:
    explicit ParamVSWidget(QWidget *parent = nullptr);
    virtual ~ParamVSWidget() override;

private:
    Ui::ParamVSWidget *ui;

    ParamPlotter_set*       paramPlotter;
    ParamVectorColor_set*   paramVectorColor;
    ParamVectorObject_set*  paramVectorObject;
    ParamVectorScan_set*    paramVectorScan;
    ParamPlotterWid_set*    paramPlotterWid;

    const int paramNumber = 5;

    // IParamWidget interface
public:
    virtual bool setParamData(QList<IParamData *> *dat) override;
private slots:
    void on_spinPixStep_valueChanged(int arg1);
    void on_spinPlotWid_valueChanged(double arg1);
    void on_sliderGridX_valueChanged(int value);
    void on_spinGridX_valueChanged(int arg1);
    void on_sliderGridY_valueChanged(int value);
    void on_spinGridY_valueChanged(int arg1);
    void on_spinNumCol_valueChanged(int arg1);
    void on_spinVelColor_valueChanged(int arg1);
    void on_butBegColor_clicked();
    void on_butEndColor_clicked();
};

#endif // PARAMVSWIDGET_H
