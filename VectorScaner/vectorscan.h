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
        VectorScan(QObject* parent, QSettings* settings);

        virtual ~VectorScan() override;


    protected slots:
        virtual void openedImage(const QImage &, QString const&) override;

        virtual void openedByteImage(const ByteImage &, QString const&) override;

        virtual void savePlot() override;

        virtual void getParamWidget(QWidget **widget) override;
    private slots:

        void closeEditor();

        void closePlotter();

        void showWidget_tr(QWidget* widget);

        void updateParam(IParamData* data);


    private:
        // Агругируем векторный редактор
        QPointer<IVectorEditor>  editor;

        // И средство для рисования графика
        QPointer<IPlotter>       plotter;

        // Еще нужно хранить заголовок для окна (имя открытого файла)
        QString windowTitle;

        ParamVectorScan* param;

        IParametres* iParam;

        bool openedIm = false, openedByteIm = false;

        bool editorClosed = false, plotterClosed = false;

    };

}

#endif // VECTORSCAN_H
