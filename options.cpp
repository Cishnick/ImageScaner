#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);

    auto mainLayout = new QGridLayout(this);
    this->setLayout(mainLayout);
    mainLayout->addWidget(ui->tabWidget);

  //  this->setFixedSize(this->size());

    // Настраиваем layouts
    ui->groupData->setLayout(ui->layoutData);
    ui->groupPlot->setLayout(ui->layoutPlot);
    ui->groupGridY->setLayout(ui->layoutGridY);
    ui->tab->setLayout(ui->layoutPlotInt);

}

Options::~Options()
{
    delete ui;
}

void Options::setBeginCalcParam(const ParamCalc &param)
{
    paramCalc = param;
    ui->spinStep->setValue(param.step);
}

void Options::setBeginPlotParam(const Param &param)
{
    paramPlot = param;
    ui->spinGridY->setValue(param.yGrid.steps);
    ui->sliderGridY->setValue(param.yGrid.steps);
    ui->spinWidthPlot->setValue(param.plotWidth);
}

void Options::on_spinWidthPlot_valueChanged(double arg1)
{
    paramPlot.plotWidth = arg1;
    emit setPlotParam(paramPlot);
}

void Options::on_sliderGridY_valueChanged(int value)
{
    paramPlot.yGrid.steps = value;
    ui->spinGridY->setValue(value);
    emit setPlotParam(paramPlot);
}

void Options::on_spinGridY_valueChanged(int arg1)
{
    paramPlot.yGrid.steps = arg1;
    ui->sliderGridY->setValue(arg1);
    emit setPlotParam(paramPlot);
}

void Options::on_spinStep_valueChanged(int arg1)
{
    paramCalc.step = arg1;
    emit setCalcParam(paramCalc);
}
