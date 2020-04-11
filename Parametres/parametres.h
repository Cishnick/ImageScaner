#ifndef PARAMETRES_H
#define PARAMETRES_H

#include "iparametres.h"
#include "iparamwidget.h"
class Parametres final : public IParametres
{
    Q_OBJECT
public:
    Parametres(QSettings* sett, IParamWidget* wid);

    void setParamData(QList<IParamData*>&& dat);

    // IParametres interface
protected slots:
    virtual void getWidget(QWidget **retWidget) override;

    virtual void updateAll() override;
private:
    QPointer<IParamWidget>      widget;
    QList<IParamData*>          dataList;
    QPointer<QSettings>         settings;
};
#endif // PARAMETRES_H
