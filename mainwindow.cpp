#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace _VectorScan;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);
    ui->actionExit->setShortcut(QKeySequence("Alt+F4"));
    ui->actionSave->setEnabled(false);
    this->setWindowIcon(QIcon("main.ico"));

    // Инициализируем многооконный интерфейс
    mdi = new QMdiArea(this);
    mdi->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    mdi->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    setCentralWidget(mdi);

    actGroup = new QActionGroup(this);

    proc = new ImageProcessorShall();

    connect(this, SIGNAL(openImage(QImage const&, QString const&)),
            proc, SLOT(setData(QImage const&, QString const&)), Qt::DirectConnection );

    connect(proc, SIGNAL(setStatusText(QString const&)),
            this, SLOT(setStatusText(QString const&)) );

    connect(mdi, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(subWindowActivated(QMdiSubWindow*)) );

    statusLabel = new QLabel(this);
    statusBar()->addWidget(statusLabel);
    setStatusText("Готов");

    optFilters = new OptionFilterWidget();

    connect(optFilters, SIGNAL(setFilter(QString const&, optionFilters*)),
            proc, SLOT(setFilter(QString const&, optionFilters*)) );

    connect(optFilters, SIGNAL(setSaveImage(bool)),
            proc, SLOT(setSaveImage(bool)) );
    qRegisterMetaType<image_t>("image_t");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showWidget(QWidget *widget)
{
    if(!widget)
        return;
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
        t->setMaximumSize(widget->maximumSize());
        t->setMinimumSize(widget->minimumSize());
    }

    auto flag = widget->windowFlags();
    t->setWindowFlag(Qt::WindowType::WindowStaysOnTopHint,
                     flag.testFlag(Qt::WindowType::WindowStaysOnTopHint));
    t->setWindowIcon(widget->windowIcon());

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
    setStatusText("Готов");
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
    static QSettings settings("Parametres.ini", QSettings::IniFormat);
    _VectorScan::IVectorScan *scaner;
    scaner = FactoryVectorScan::create(this, &settings);

    connect(proc, SIGNAL(processed(QImage const&, QString const&)),
            scaner, SLOT(openedImage(QImage const&, QString const&)), Qt::QueuedConnection );

    connect(proc, SIGNAL(byteImage(image_t const&, QString const&)),
            scaner, SLOT(openedByteImage(image_t const&, QString const&)), Qt::QueuedConnection );

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

    connect(this, SIGNAL(getWidget(QWidget**)),
            scaner, SLOT(getParamWidget(QWidget**)) );

    scanerList.push_back(scaner);
    setStatusText("Готов");
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

void MainWindow::setStatusText(const QString &text)
{
    statusLabel->setText(text);
}

void MainWindow::openImage_sl(const QImage &img, const QString &str)
{
    emit openImage(img, str);
    setStatusText("Обработка изображения ... ");
    proc->process();
}
//  ---------------------------------------------- Меню -------------------------
void MainWindow::on_actionOpen_triggered()
{
    setStatusText("Открытие файла...");
    auto fname = QFileDialog::getOpenFileName(this,
                                             "Открыть изображение",
                                             _dialogDir,
                                              "*.bmp *.tiff *.jpg *.jpeg *.png");
    if(!fname.isEmpty())
    {
        auto image = QImage(fname);
        openedImage();
        openImage_sl(image, fname);
    }
    else
    {
        setStatusText("Готов");
    }
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
        //    if(!closedWidgets.contains(t->widget()))
        //    {
        //        closedWidgets.push_back(t->widget());
        //    }
        t->setWidget(nullptr);
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
        setStatusText("Перетащите файл в окно программы и отпустите");
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
            openedImage();
            openImage_sl(QImage(filename), filename);
            event->acceptProposedAction();
            return;
        }
    }

    return;
}

void MainWindow::on_action_Copy_triggered()
{
    if(mdi->subWindowList().size() == 0)
        return;
    auto wid = mdi->activeSubWindow()->widget();
    QPixmap pixmap(wid->size());
    wid->render(&pixmap);
    QApplication::clipboard()->setPixmap(pixmap);
    setStatusText("Скопировано содержимое активного окна");
}

void MainWindow::on_action_Paste_triggered()
{
    auto img = QApplication::clipboard()->image();
    static int number = 1;
    if(!img.isNull())
    {
        openedImage();
        openImage_sl(img, "Copy_" + QString::number(number));
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

void MainWindow::on_actionOption_triggered()
{
    QWidget *widget = nullptr;
    emit getWidget(&widget);
    showWidget(widget);
}

void MainWindow::on_actionOptionFilters_triggered()
{
    showWidget(optFilters);
}
