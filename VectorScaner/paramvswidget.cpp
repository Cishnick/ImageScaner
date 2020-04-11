#include "paramvswidget.h"
#include "ui_paramvswidget.h"

ParamVSWidget::ParamVSWidget(QWidget *parent) :
    IParamWidget(parent),
    ui(new Ui::ParamVSWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("Параметры");

    this->setLayout(ui->layoutForm);
    ui->groupEdit->setLayout(ui->layoutEdit);
    ui->groupPlot->setLayout(ui->layoutPlot);
    ui->groupGridX->setLayout(ui->layoutGridX);
    ui->groupGridY->setLayout(ui->layoutGridY);
}

ParamVSWidget::~ParamVSWidget()
{
    delete ui;
}

bool ParamVSWidget::setParamData(QList<IParamData *> *dat)
{
    if(dat->size() != paramNumber)
        return false;
    auto it = dat->begin();
    paramPlotter        = dynamic_cast<ParamPlotter_set*>(*it++);
    paramVectorColor    = dynamic_cast<ParamVectorColor_set*>(*it++);
    paramVectorObject   = dynamic_cast<ParamVectorObject_set*>(*it++);
    paramVectorScan     = dynamic_cast<ParamVectorScan_set*>(*it++);
    paramPlotterWid     = dynamic_cast<ParamPlotterWid_set*>(*it++);

    QPalette palette = ui->butBegColor->palette();
    palette.setColor(ui->butBegColor->backgroundRole(), paramVectorObject->colorBeginPoint());
    ui->butBegColor->setAutoFillBackground(true);
    ui->butBegColor->setPalette(palette);

    palette = ui->butEndColor->palette();
    palette.setColor(ui->butEndColor->backgroundRole(), paramVectorObject->colorEndPoint());
    ui->butEndColor->setAutoFillBackground(true);
    ui->butEndColor->setPalette(palette);

    return paramPlotter && paramVectorColor && paramVectorObject &&
            paramVectorScan && paramPlotterWid;
}

void ParamVSWidget::on_spinPixStep_valueChanged(int arg1)
{
    paramPlotter->setStep(arg1);
    emit updateParam(paramPlotter);
}

void ParamVSWidget::on_spinPlotWid_valueChanged(double arg1)
{
    paramPlotterWid->setPlotWidth(arg1);
    emit updateParam(paramPlotterWid);
}

void ParamVSWidget::on_sliderGridX_valueChanged(int value)
{
    ui->spinGridX->setValue(value);
    auto grid = paramPlotterWid->xGrid();
    grid.steps = value;
    paramPlotterWid->setXGrid(grid);
    emit updateParam(paramPlotterWid);
}

void ParamVSWidget::on_spinGridX_valueChanged(int arg1)
{
    ui->sliderGridX->setValue(arg1);
    auto grid = paramPlotterWid->xGrid();
    grid.steps = arg1;
    paramPlotterWid->setXGrid(grid);
    emit updateParam(paramPlotterWid);
}

void ParamVSWidget::on_sliderGridY_valueChanged(int value)
{
    ui->spinGridY->setValue(value);
    auto grid = paramPlotterWid->yGrid();
    grid.steps = value;
    paramPlotterWid->setYGrid(grid);
    emit updateParam(paramPlotterWid);
}

void ParamVSWidget::on_spinGridY_valueChanged(int arg1)
{
    ui->sliderGridY->setValue(arg1);
    auto grid = paramPlotterWid->yGrid();
    grid.steps = arg1;
    paramPlotterWid->setYGrid(grid);
    emit updateParam(paramPlotterWid);
}

void ParamVSWidget::on_spinNumCol_valueChanged(int arg1)
{
    paramVectorColor->setNumber(arg1);
    emit updateParam(paramVectorColor);
}

void ParamVSWidget::on_spinVelColor_valueChanged(int arg1)
{
    paramVectorColor->setVelocity(arg1);
    emit updateParam(paramVectorColor);
}

void ParamVSWidget::on_butBegColor_clicked()
{
    QColor color = paramVectorObject->colorBeginPoint();
    color = QColorDialog::getColor(color, this);
    if(color.isValid())
    {
        paramVectorObject->setColorBeginPoint(color);
        QPalette palette = ui->butBegColor->palette();
        palette.setColor(ui->butBegColor->backgroundRole(), paramVectorObject->colorBeginPoint());
        ui->butBegColor->setAutoFillBackground(true);
        ui->butBegColor->setPalette(palette);
        emit updateParam(paramVectorObject);
    }
}

void ParamVSWidget::on_butEndColor_clicked()
{
    QColor color = paramVectorObject->colorEndPoint();
    color = QColorDialog::getColor(color, this);
    if(color.isValid())
    {
        paramVectorObject->setColorEndPoint(color);
        QPalette palette = ui->butEndColor->palette();
        palette.setColor(ui->butEndColor->backgroundRole(), paramVectorObject->colorEndPoint());
        ui->butEndColor->setAutoFillBackground(true);
        ui->butEndColor->setPalette(palette);
        emit updateParam(paramVectorObject);
    }
}
