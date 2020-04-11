#ifndef FACTORYPARAMETRESVS_H
#define FACTORYPARAMETRESVS_H

#include "Parametres/iparametres.h"
#include "Parametres/parametres.h"
#include "paramdata_set.h"
#include "paramvswidget.h"

class FactoryParametresVS : IFactoryParametres
{
public:
    virtual IParametres *create(QSettings *settings) override
    {
        QList<IParamData*>list;
        list.push_back(new ParamPlotter_set(settings));
        list.push_back(new ParamVectorColor_set(settings));
        list.push_back(new ParamVectorObject_set(settings));
        list.push_back(new ParamVectorScan_set(settings));
        list.push_back(new ParamPlotterWid_set(settings));
        ParamVSWidget *widget = new ParamVSWidget();
        Parametres *parametres = new Parametres(settings, widget);
        parametres->setParamData(std::move(list));
        return parametres;
    }
};


#endif // FACTORYPARAMETRESVS_H
