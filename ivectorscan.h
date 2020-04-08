#ifndef IVECTORSCAN_H
#define IVECTORSCAN_H
#include <QObject>
#include <QWidget>
#include "totaldefines.h"
#include <parametres.h>

// Пространство имен разделяет между собой основные модули программы
namespace _VectorScan
{

// ----------------------- IVectorScan -------------------------------------
    class IVectorScan : public QObject
    {
        Q_OBJECT
    public:
        // Конструктор нужен, т.к. он явно вызывается из наследника
        IVectorScan(QObject* parent = nullptr) : QObject(parent) {}
        virtual ~IVectorScan() = default;

    signals:

        // Сигнал, который посылается для отображения виджета
        void showWidget(QWidget* widget);

            // Пока что не юзаются!!!
        void getImage();

        void getByteImage();

        // Испускается при закрытии всех окон
        void closed(QObject*);

        void closedPlotter();

        // Испускается при закрытии окна
        void closeWindow(QWidget*);

        // Испускается перед открытием окна с графиком
        void openPloter();

        // Испускается перед открытием окна с редактором
        void openEditor();

    public slots:

        // По факту, слот запуска модуля
        virtual void openedImage(QImage const&, QString const&) = 0;

        virtual void openedByteImage(ByteImage const&, QString const&) = 0;

        virtual void savePlot() = 0;

        // Говорит открыть окно с параметрами графика
        virtual void openWindowOption() = 0;
    };

// ----------------------------------- FactoryVectorScan --------------------
    class FactoryVectorScan
    {
    public:
        // Создает экземпляр
        static IVectorScan* create(QObject* parent = nullptr);
    };
}
#endif // IVECTORSCAN_H
