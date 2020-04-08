#ifndef VECTORWIDGET_H
#define VECTORWIDGET_H

#include "graphicclasses.h"
#include <QStatusBar>
#include <QMouseEvent>
#include <QLabel>
#include <QPushButton>
#include <QMainWindow>
#include "parametres.h"
namespace _VectorScan
{

    class VectorWidget : public QMainWindow
    {
        Q_OBJECT
    public:
        VectorWidget(QWidget* parent = nullptr);

        virtual ~VectorWidget() override = default;
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

    public:
        void start(const QImage &image, QString const& fname);

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

        // QWidget interface
    protected:
        virtual void closeEvent(QCloseEvent *event) override;
    };


}
#endif // VECTORWIDGET_H
