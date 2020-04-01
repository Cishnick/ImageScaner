#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "iimageopener.h"
#include "ivectorscan.h"
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QDebug>
#include <QMessageBox>
#include <QActionGroup>
#include <QClipboard>
#include <QMap>
#include <QMimeData>
#include <QVariant>

namespace Ui {
class MainWindow;
}

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

    // Сигнал открытия файла
    void openFile();

    void openImage(QImage const&);

    void savePlot();

private slots:
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

private:
    Ui::MainWindow *ui;

    // Модуль для открытия файла
    _ImageOpener::IImageOpener*         opener;

    // Модуль для сканирования открытого файла
    QList<_VectorScan::IVectorScan*>    scanerList;

    // Для многооконного интерфейса
    QMdiArea*                           mdi;

    QActionGroup*                       actGroup;

    QMap<QAction*, QMdiSubWindow*>      windowMenu;


    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    // QWidget interface
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) override;

    virtual void dropEvent(QDropEvent *event) override;

};

#endif // MAINWINDOW_H
