#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "VectorScaner\ivectorscan.h"
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QDebug>
#include <QMessageBox>
#include <QActionGroup>
#include <QClipboard>
#include <QMap>
#include <QMimeData>
#include <QVariant>
#include "imageprocessorshall.h"
#include <QFileDialog>
#include <QLabel>
#include <QStatusBar>
#include <QThread>
#include "OptionFilterWidget.h"

namespace Ui {
class MainWindow;
}
Q_DECLARE_METATYPE(image_t)
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    virtual ~MainWindow() override;

private slots:
    // Слот для отображения нового окна в программе
    void showWidget(QWidget* widget);

    // Обрабатывает закрытие модуля
    void closedUnit(QObject* unit);

    void openedPlotter();

    void closedPlotter();

    void openedImage();

    void triggeredActionWindow();

    void subWindowActivated(QMdiSubWindow *window);
signals:

    void openImage(QImage const&, QString const&);

    void savePlot();

    void getWidget(QWidget**);

private slots:

    void setStatusText(QString const& text);

    // Слот для пункта меню open
    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_actionSave_triggered();

    void on_action_About_triggered();

    void on_actionCascade_triggered();

    void on_actionVectical_triggered();

    void on_action_Copy_triggered();

    void on_action_Paste_triggered();

    void on_actionClose_triggered();

    void on_actionClose_All_triggered();

    void on_actionOption_triggered();

    void on_actionOptionFilters_triggered();

private:

    void openImage_sl(QImage const&, QString const&);

    Ui::MainWindow *ui;

    ImageProcessorShall*                proc;

    // Модуль для сканирования открытого файла
    QList<_VectorScan::IVectorScan*>    scanerList;

    // Для многооконного интерфейса
    QMdiArea*                           mdi;

    QActionGroup*                       actGroup;

    QMap<QAction*, QMdiSubWindow*>      windowMenu;

    QLabel*                             statusLabel;

    QThread                             thread;

    OptionFilterWidget*                 optFilters;

    QList<QWidget*>                     closedWidgets;

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    // QWidget interface
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) override;

    virtual void dropEvent(QDropEvent *event) override;

};

#endif // MAINWINDOW_H
