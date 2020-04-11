#include "parametres.h"

Parametres::Parametres(QSettings *sett, IParamWidget *wid) :
    widget(wid), settings(sett)
{
    connect(widget, SIGNAL(updateParam(IParamData*)),
            this,   SIGNAL(updateParam(IParamData*)) );
}

// ------------------------ addParamData ----------------------------------
void Parametres::setParamData(QList<IParamData*>&& dat)
{
    dataList = dat;
    widget->setParamData(&dataList);
}

// ------------------------ getWidget ----------------------------------
void Parametres::getWidget(QWidget **retWidget)
{
    *retWidget = widget;
}

// ------------------------ updateAll ----------------------------------
void Parametres::updateAll()
{
    for(auto dat : dataList)
    {
        emit updateParam(dat);
    }
}
