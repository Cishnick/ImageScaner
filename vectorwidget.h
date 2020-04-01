#ifndef VECTORWIDGET_H
#define VECTORWIDGET_H

#include "vectoreditor.h"
#include "graphicclasses.h"
#include <QStatusBar>
#include <QMouseEvent>

namespace _VectorScan
{
// Реализация IVectorWidget в отдельном *.h файле
    class VectorWidget : public IVectorWidget
    {
        Q_OBJECT
    public:
        VectorWidget(QWidget* parent = nullptr);

        virtual ~VectorWidget() override = default;

        // IVectorWidget interface
    public:
        virtual void start(const QImage &image, QString const& fname) override;

    private slots:

        void statB_clicked();

        // Эти 2 слота нужны для преобразования координат от сцены к виджету
        void vectorPainted_tr(Vector const& v);

        void vectorChanged_tr(Vector const& v);

    private:
        VectorScene*    scene;
        VectorObject*   vector;
        VectorView*     view;

        QLabel*         statLSize;
        QLabel*         statLPos;
        QPushButton*    statBColor;

        // QObject interface
    public:

        virtual bool eventFilter(QObject *watched, QEvent *event) override;
    };


}
#endif // VECTORWIDGET_H
