#ifndef VECTOREDITOR_H
#define VECTOREDITOR_H
#include "ivectoreditor.h"
#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include "vectorwidget.h"

namespace _VectorScan
{
// ---------------------------------- VectorEditor --------------------------------
    class VectorEditor : public IVectorEditor
    {
        Q_OBJECT
    public:
        VectorEditor(QObject* parent = nullptr) : IVectorEditor(parent)
        {
            widget = new VectorWidget(nullptr);

            // Подключаем сигнал создания виджета
            connect(widget, SIGNAL(created(QWidget*)),
                    this, SIGNAL(showWidget(QWidget*)) );

            connect(widget, SIGNAL(vectorPainted(Vector const&)),
                    this, SIGNAL(vectorPainted(Vector const&)) );

            connect(widget, SIGNAL(vectorChanged(Vector const&)),
                    this, SIGNAL(vectorChanged(Vector const&)) );

            connect(widget, SIGNAL(vectorRemove(QColor)),
                    this, SIGNAL(vectorRemove(QColor)) );

            connect(widget, SIGNAL(closed()),
                    this, SIGNAL(closed()) );
        }

        virtual ~VectorEditor() override
        {
            if(widget)
                delete widget;
        }

    public slots:
        virtual void openedImage(const QImage &image, QString const& fname) override
        {
            widget->start(image, fname);
        }

    private:
        // Агрегируем виджет для редактора
        QPointer<VectorWidget> widget;
    };
}

#endif // VECTOREDITOR_H
