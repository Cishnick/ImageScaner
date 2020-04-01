#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iimageopener.h"

using namespace _VectorScan;
using namespace _ImageOpener;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);
    ui->actionExit->setShortcut(QKeySequence("Alt+F4"));
    ui->actionSave->setEnabled(false);

    // Инициализируем многооконный интерфейс
    mdi = new QMdiArea(this);
    setCentralWidget(mdi);

    actGroup = new QActionGroup(this);

    // Фабрикуем необходимые модули
    opener = FactoryImageOpener::create(this);

    // Слот для открытия файла
    connect(this, SIGNAL(openFile()),
            opener, SLOT(openFile()) );

    connect(opener, SIGNAL(openedImage()),
            this,  SLOT(openedImage()) );

    connect(this, SIGNAL(openImage(QImage const&)),
            opener, SLOT(openImage(QImage const&)) );

    connect(mdi, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(subWindowActivated(QMdiSubWindow*)) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showWidget(QWidget *widget)
{
    QMdiSubWindow* t = mdi->addSubWindow(widget);

    // Если виджет имеет фиксированный размер
    if(widget->maximumSize() == widget->minimumSize())
    {
        t->setFixedSize(t->size());
        t->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    }
    else
    {
        t->resize(widget->size());
    }

    auto flag = widget->windowFlags();
    t->setWindowFlag(Qt::WindowType::WindowStaysOnTopHint,
                     flag.testFlag(Qt::WindowType::WindowStaysOnTopHint));

    t->installEventFilter(this);
    auto act = new QAction(this);
    act->setCheckable(true);
    act->setActionGroup(actGroup);
    act->setChecked(true);
    connect(act, SIGNAL(triggered()),
            this, SLOT(triggeredActionWindow()) );
    act->setText(t->windowTitle());
    ui->menuWindow->addAction(act);
    windowMenu[act] = t;

    widget->show();
}

void MainWindow::closedUnit(QObject *unit)
{
    auto temp = dynamic_cast<_VectorScan::IVectorScan*>(unit);
    if(temp)
    {
        scanerList.removeOne(temp);
        delete temp;
    }
}

void MainWindow::openedPlotter()
{
    ui->actionSave->setEnabled(true);
}

void MainWindow::closedPlotter()
{
    ui->actionSave->setEnabled(false);
}

void MainWindow::openedImage()
{
    _VectorScan::IVectorScan *scaner;
    scaner = FactoryVectorScan::create(this);

    // Слоты между сканером и открывашкой
    connect(scaner, SIGNAL(getImage()),
            opener, SLOT(getImage()) );

    connect(scaner, SIGNAL(getByteImage()),
            opener, SLOT(getByteImage()) );

    connect(opener, SIGNAL(openedImage(QImage const&)),
            scaner, SLOT(openedImage(QImage const&)) );

    connect(opener, SIGNAL(openedByteImage(ByteImage const&)),
            scaner, SLOT(openedByteImage(ByteImage const&)) );

    connect(scaner, SIGNAL(getLastFileName()),
            opener, SLOT(getLastFileName()) );

    connect(opener, SIGNAL(lastFileName(QString const&)),
            scaner, SLOT(LastFileName(QString const&)) );

    connect(scaner, SIGNAL(closed(QObject*)),
            this, SLOT(closedUnit(QObject*)) );

    connect(scaner, SIGNAL(openPloter()),
            this, SLOT(openedPlotter()) );

    connect(scaner, SIGNAL(closedPlotter()),
            this, SLOT(closedPlotter()) );

    connect(this, SIGNAL(savePlot()),
            scaner, SLOT(savePlot()) );

    // Соединяем слот для показа нового окна
    connect(scaner, SIGNAL(showWidget(QWidget*)),
            this, SLOT(showWidget(QWidget*)) );

    scanerList.push_back(scaner);
}

void MainWindow::triggeredActionWindow()
{
    QAction* act = dynamic_cast<QAction*>(sender());
    mdi->setActiveSubWindow(windowMenu[act]);
}

void MainWindow::subWindowActivated(QMdiSubWindow *window)
{
    for(auto key : windowMenu.keys())
    {
        if(windowMenu[key] == window)
        {
            key->setChecked(true);
            break;
        }
    }
}

//  ---------------------------------------------- Меню -------------------------
void MainWindow::on_actionOpen_triggered()
{
    emit openFile();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionSave_triggered()
{
    emit savePlot();
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox::about(this, " О программе", "ImageScaner, ver 1.0.0\n");
}

void MainWindow::on_actionCascade_triggered()
{
    mdi->cascadeSubWindows();
}

void MainWindow::on_actionVectical_triggered()
{
    mdi->tileSubWindows();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    auto t = dynamic_cast<QMdiSubWindow*>(watched);
    if(t && event->type() == QEvent::Close)
    {
        for(auto key : windowMenu.keys())
        {
            if(windowMenu[key] == t)
            {
                windowMenu.remove(key);
                actGroup->removeAction(key);
                ui->menuWindow->removeAction(key);
                break;
            }
        }

    }
    return false;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QStringList pathList;
    QList<QUrl> urlList = event->mimeData()->urls();
    for(auto url : urlList)
    {
        auto filename = url.toLocalFile();
        auto exp = filename.split('.').last();
        if(_fileFormats.contains(exp))
        {
            qDebug() << filename;
            emit openImage(QImage(filename));
            event->acceptProposedAction();
            return;
        }
    }

    return;
}

void MainWindow::on_action_Copy_triggered()
{
    auto wid = mdi->activeSubWindow()->widget();
    QPixmap pixmap(wid->size());
    wid->render(&pixmap);
    QApplication::clipboard()->setPixmap(pixmap);
}

void MainWindow::on_action_Paste_triggered()
{
    auto img = QApplication::clipboard()->image();
    if(!img.isNull())
    {
        emit openImage(img);
    }
}

void MainWindow::on_actionClose_triggered()
{
    mdi->closeActiveSubWindow();
}

void MainWindow::on_actionClose_All_triggered()
{
    mdi->closeAllSubWindows();
}
