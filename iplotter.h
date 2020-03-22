#ifndef IPLOTTER_H
#define IPLOTTER_H

#include <QObject>

class IPlotter : public QObject
{
    Q_OBJECT
public:
    explicit IPlotter(QObject *parent = nullptr);

signals:

public slots:
};

#endif // IPLOTTER_H
