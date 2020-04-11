#ifndef IPARAMWIDGET_H
#define IPARAMWIDGET_H

#include <QWidget>
#include "iparamdata.h"
class IParamWidget : public QWidget
{
    Q_OBJECT
protected:
    IParamWidget(QWidget* parent = nullptr) : QWidget(parent) {}

    virtual ~IParamWidget() = default;

public:
    // Инициализирует массив с данными
    virtual bool setParamData(QList<IParamData*>* dat) = 0;

protected:

signals:
    void updateParam(IParamData*);
};
#endif // IPARAMWIDGET_H
