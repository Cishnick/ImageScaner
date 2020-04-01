#ifndef VECTOREDITOR_H
#define VECTOREDITOR_H
#include "ivectoreditor.h"
#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

namespace _VectorScan
{

// ------------------------------------ IVectorWidget ------------------------------
    class IVectorWidget : public QMainWindow
    {
        Q_OBJECT
    public:
        IVectorWidget(QWidget* parent = nullptr) : QMainWindow(parent) {}
        virtual ~IVectorWidget() override{}

        // Запускает виджет, получая на вход изображение
        virtual void start(QImage const& image, QString const& fname) = 0;

    signals:
        // Отправляется сразу после создания, аргумент - this
        void created(QWidget*);

        // Отправляется после установки первой точки, когда рисуется вектор
        void vectorPainted(Vector const& vect);

        // Отправляется при изменении параметров какого-либо вектора
        void vectorChanged(Vector const& vect);

        // Удаляет вектор с указанным цветом
        void vectorRemove(QColor color);

        // Испускается при закрытии окна
        void closed();

        // QWidget interface
    protected:
        virtual void closeEvent(QCloseEvent *event) override;
    };

// ---------------------------------- FabricWidget --------------------------------
    class FactoryWidget
    {
    public:
        static IVectorWidget* create(QWidget* parent);
    };

// ---------------------------------- VectorEditor --------------------------------
    class VectorEditor : public IVectorEditor
    {
        Q_OBJECT
    public:
        VectorEditor(QObject* parent = nullptr) : IVectorEditor(parent)
        {
            widget = FactoryWidget::create(nullptr);

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
        QPointer<IVectorWidget> widget;
    };
}

#endif // VECTOREDITOR_H
