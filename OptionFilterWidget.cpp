#include "OptionFilterWidget.h"
#include "ui_form.h"

OptionFilterWidget::OptionFilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose, false);

    auto layout = new QGridLayout();
    this->setLayout(layout);
    layout->addWidget(ui->groupFilter);
    ui->groupFilter->setLayout(ui->layoutFilter);
    ui->groupOption->setLayout(ui->layoutOption);

    ui->wFilterNo->setLayout(ui->layoutFIlterNo);
    ui->wFilterMedian->setLayout(ui->layoutFilterMedian);
    ui->wFilterBilateral->setLayout(ui->layoutFIlterBilateral);
    ui->wFilterMedian->setVisible(false);
    ui->wFilterBilateral->setVisible(false);

    optMed.windowSize = 5;

    optBil.windowSize = 5;
    optBil.sigmaI = 25.;
    optBil.sigmaR = 16.;

}

OptionFilterWidget::~OptionFilterWidget()
{
    delete ui;
}

void OptionFilterWidget::on_comboFilter_activated(int index)
{
    switch(index)
    {
    case 0:
        emit setFilter("", nullptr);
        ui->checkSave->setEnabled(false);
        ui->wFilterNo->setVisible(true);
        ui->wFilterMedian->setVisible(false);
        ui->wFilterBilateral->setVisible(false);
        break;
    case 1:
        emit setFilter(medName, &optMed);
        ui->checkSave->setEnabled(true);
        ui->wFilterNo->setVisible(false);
        ui->wFilterMedian->setVisible(true);
        ui->wFilterBilateral->setVisible(false);
        break;
    case 2:
        emit setFilter(bilName, &optBil);
        ui->checkSave->setEnabled(true);
        ui->wFilterNo->setVisible(false);
        ui->wFilterMedian->setVisible(false);
        ui->wFilterBilateral->setVisible(true);
        break;
    }
    emit setSaveImage(ui->checkSave->isChecked());
}

void OptionFilterWidget::on_spinMedKernelSize_valueChanged(int arg1)
{
    if(!(arg1 % 2))
    {
        ui->spinMedKernelSize->setValue(arg1 - 1);
    }
    optMed.windowSize = arg1;
    emit setFilter(medName, &optMed);
}

void OptionFilterWidget::on_spinBilKernelSIze_valueChanged(int arg1)
{
    if(!(arg1 % 2))
    {
        ui->spinBilKernelSIze->setValue(arg1 - 1);
    }
    optBil.windowSize = arg1;
    emit setFilter(medName, &optBil);
}

void OptionFilterWidget::on_spinSi_valueChanged(double arg1)
{
    optBil.sigmaI = arg1;
    emit setFilter(medName, &optBil);
}

void OptionFilterWidget::on_spinSr_valueChanged(double arg1)
{
    optBil.sigmaR = arg1;
    emit setFilter(medName, &optBil);
}

void OptionFilterWidget::on_checkSave_toggled(bool checked)
{
    emit setSaveImage(checked);
}
