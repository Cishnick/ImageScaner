#ifndef IVECTOREDITOR_H
#define IVECTOREDITOR_H

#include <QObject>

class IVectorEditor : public QObject
{
    Q_OBJECT
public:
    explicit IVectorEditor(QObject *parent = nullptr);

signals:

public slots:
};

#endif // IVECTOREDITOR_H
