#include "vectorscan.h"

using namespace _VectorScan;

// -------------------------------- VectorScan::VectorScan ---------------------------------
_VectorScan::VectorScan::VectorScan(QObject *parent) :
    IVectorScan(parent)
{
    // Создает реализации подмодулей
    editor  = FactoryVectorEditor::create(this);
    plotter = FactoryPlotter::create(this);

    // И коннектим все нужные слоты и сигналы
    connect(editor, SIGNAL(showWidget(QWidget*)),
            this, SLOT(showWidget_tr(QWidget*)) );

    connect(plotter, SIGNAL(showWidget(QWidget*)),
            this, SLOT(showWidget_tr(QWidget*)) );

    connect(editor, SIGNAL(vectorPainted(Vector const&)),
            plotter, SLOT(vectorPainted(Vector const&)), Qt::QueuedConnection );

    // SAVE!
    connect(editor, SIGNAL(vectorChanged(Vector const&)),
            plotter, SLOT(vectorChanged(Vector const&)), Qt::QueuedConnection );

    connect(editor, SIGNAL(vectorRemove(QColor)),
            plotter, SLOT(vectorRemove(QColor)), Qt::QueuedConnection );

    connect(editor, SIGNAL(closed()),
            this, SLOT(closeEditor()) );

    connect(plotter, SIGNAL(closed()),
            this, SLOT(closePlotter()) );

    connect(this, SIGNAL(openWindowOption_sg()),
            plotter, SLOT(openWindowOption()) );

}

// ----------------------------------- ~VectorScan ---------------------------------------
VectorScan::~VectorScan()
{
    if(editor)
        delete editor;
    if(plotter)
        delete plotter;
}

// ----------------------------------- openedImage ---------------------------------------
void VectorScan::openedImage(const QImage &image, QString const& fname)
{
    if(openedIm)
        return;

    windowTitle = fname;
    editor->openedImage(image, fname);

    openedIm = true;
}

// ----------------------------------- openedByteImage -----------------------------------
void VectorScan::openedByteImage(const ByteImage &image, QString const& fname)
{
    if(openedByteIm)
        return;
    windowTitle = fname;
    plotter->openedByteImage(image, fname);

    openedByteIm = true;
}

// ---------------------------------- savePlot --------------------------------------
void VectorScan::savePlot()
{
    QImage screen = plotter->plotImage();
    auto fileName = QFileDialog::getSaveFileName(nullptr, "Сохранить "+ windowTitle, _dialogDir,
                                 "Image(*.bmp *.jpg *.jpeg *.png *.tiff)");
    if(!fileName.isEmpty())
    {
        screen.save(fileName);
    }
}

// ---------------------------------- openWindowOption ----------------------------------
void VectorScan::openWindowOption()
{
    emit openWindowOption_sg();
}

// ---------------------------------- closeEditor --------------------------------------
void VectorScan::closeEditor()
{
    editorClosed = true;
    if(plotterClosed)
        emit closed(static_cast<QObject*>(this));
}

// ---------------------------------- LastFileName --------------------------------------
void VectorScan::closePlotter()
{
    plotterClosed = true;
    emit closedPlotter();
    if(editorClosed)
        emit closed(static_cast<QObject*>(this));
}

// ---------------------------------- showWidget_tr --------------------------------------
void VectorScan::showWidget_tr(QWidget* widget)
{
    if(dynamic_cast<IVectorEditor*>(sender()))
    {
        emit openEditor();
    }
    else if(dynamic_cast<IPlotter*>(sender()))
    {
        emit openPloter();
    }
    emit showWidget(widget);
}

// ------------------------------------ Реализация фабрики -----------------------------
IVectorScan* FactoryVectorScan::create(QObject* parent)
{
    return new VectorScan(parent);
}
