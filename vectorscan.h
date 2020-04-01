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

    protected slots:
        virtual void openedImage(const QImage &) override;

        virtual void openedByteImage(const ByteImage &) override;

        virtual void LastFileName(const QString &filename) override;

        virtual void savePlot() override;

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
