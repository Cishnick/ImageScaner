#ifndef VECTORSCAN_H
#define VECTORSCAN_H
#include "ivectorscan.h"
#include "ivectoreditor.h"
#include "iplotter.h"
#include <QFileDialog>
/*
 * Отдельный заголовочный файл, для объявления реализации интерфейса.
 * Этот класс надо объявлять в *.h файле, чтобы корректно работа MOC
 */

namespace _VectorScan
{
// ------------------------------------- VectorScan ---------------------------
    class VectorScan : public IVectorScan
    {
        Q_OBJECT
    public:
        VectorScan(QObject* parent = nullptr);

        virtual ~VectorScan() override;

    signals:
        void openWindowOption_sg();

    protected slots:
        virtual void openedImage(const QImage &, QString const&) override;

        virtual void openedByteImage(const ByteImage &, QString const&) override;

        virtual void savePlot() override;

        virtual void openWindowOption() override;

    private slots:

        void closeEditor();

        void closePlotter();

        void showWidget_tr(QWidget* widget);


    private:
        // Агругируем векторный редактор
        QPointer<IVectorEditor>  editor;

        // И средство для рисования графика
        QPointer<IPlotter>       plotter;

        // Еще нужно хранить заголовок для окна (имя открытого файла)
        QString windowTitle;

        bool openedIm = false, openedByteIm = false;

        bool editorClosed = false, plotterClosed = false;

    };

}

#endif // VECTORSCAN_H
