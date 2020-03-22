#ifndef IMAGEOPENER_H
#define IMAGEOPENER_H

#include <QObject>

class IImageOpener : public QObject
{
    Q_OBJECT
public:
    explicit IImageOpener(QObject *parent = nullptr);

signals:

public slots:
};

#endif // IMAGEOPENER_H
