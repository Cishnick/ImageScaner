#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include "ImageProcessor/defines.h"
namespace Ui {
class Form;
}

class OptionFilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OptionFilterWidget(QWidget *parent = nullptr);
    ~OptionFilterWidget();

signals:

    void setFilter(QString const& name, optionFilters* option);

    void setSaveImage(bool);

private slots:
    void on_comboFilter_activated(int index);

    void on_spinMedKernelSize_valueChanged(int arg1);

    void on_spinBilKernelSIze_valueChanged(int arg1);

    void on_spinSi_valueChanged(double arg1);

    void on_spinSr_valueChanged(double arg1);


    void on_checkSave_toggled(bool checked);

private:
    optionFilterMedian optMed;
    optionFilterBilateral optBil;

    const QString medName = "Median Filter";
    const QString bilName = "Bilateral Filter";

    Ui::Form *ui;

};

#endif // FORM_H
