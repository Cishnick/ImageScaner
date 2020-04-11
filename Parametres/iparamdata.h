#ifndef IPARAMDATA_H
#define IPARAMDATA_H
#include <QSettings>
#include <QPointer>
class IParamData
{
public:
    // Сохраняет данные в settings
    virtual void saveData() = 0;

    IParamData(QSettings* settings) : settings(settings) {}
    virtual ~IParamData() = default;
protected:
    QPointer<QSettings> settings;
};
#endif // IPARAMDATA_H
