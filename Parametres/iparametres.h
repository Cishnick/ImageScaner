#ifndef IPARAMETRES_H
#define IPARAMETRES_H
#include <QObject>
#include "iparamdata.h"
#include "iparamwidget.h"
class IParametres: public QObject
{
    Q_OBJECT
public:

signals:
    void updateParam(IParamData* data);

public slots:

    // Записывает в аргумент указатель на виджет с настройками параметров
    virtual void getWidget(QWidget** retWidget) = 0;

    // Высылает сигнал обновления для всех параметров
    virtual void updateAll() = 0;

};


class IFactoryParametres
{
public:
    virtual IParametres* create(QSettings* settings) = 0;

protected:
    QSettings* settings;
};



#endif // IPARAMETRES_H
