#include "plotterwid.h"
using namespace _VectorScan;

PlotterWid::PlotterWid(QWidget *parent) :
    IPlotterWid(parent)
{
    // Начальный размер просто от балды)
    this->resize(650, 400);

    this->setMinimumSize(325, 200);
    this->setWindowFlag(Qt::WindowType::WindowStaysOnTopHint, true);

    // Заполняем параметры графика
    param.xAxe.color    = Qt::GlobalColor::black;
    param.xAxe.label    = "X";
    param.xAxe.width    = 1;
    param.xAxe.isArrow  = false;
    param.xAxe.penStyle = Qt::PenStyle::SolidLine;

    param.yAxe.color     = Qt::GlobalColor::black;
    param.yAxe.label     = "Y";
    param.yAxe.width     = 1;
    param.yAxe.isArrow   = false;
    param.yAxe.penStyle  = Qt::PenStyle::SolidLine;

    param.xGrid.beginVal  = 0;
    param.xGrid.endVal    = 100;
    param.xGrid.steps     = 1; // Не выставлять в 0! Иначе не скомпилится
    param.yGrid.widthStep = 100;
    param.xGrid.penStyle  = Qt::PenStyle::DashLine;
    param.xGrid.colorLine = Qt::GlobalColor::gray;
    param.xGrid.widthLine = 1;
    param.xGrid.colorText = Qt::GlobalColor::black;
    param.xGrid.textFont  = QFont("Times New Roman", 10);

    param.yGrid.beginVal  = 0;
    param.yGrid.endVal    = 255;
    param.yGrid.steps     = 255/30;
    param.yGrid.widthStep = 30;
    param.yGrid.penStyle  = Qt::PenStyle::DashDotLine;
    param.yGrid.colorLine = Qt::GlobalColor::darkGray;
    param.yGrid.widthLine = 1;
    param.yGrid.colorText = Qt::GlobalColor::black;
    param.yGrid.textFont  = QFont("Times New Roman", 10);

    param.margin.left   = 30;
    param.margin.right  = 30;
    param.margin.top    = 30;
    param.margin.bottom = 30;

    param.rectGraph = QRect(param.margin.left, param.margin.top,
                       this->size().width() - param.margin.left-param.margin.right,
                       this->size().height() - param.margin.top-param.margin.bottom);

    param.plotWidth = 1.6;
    param.plotPenStyle = Qt::PenStyle::SolidLine;

    cMenu = new QMenu(this);

    actHideAll = new QAction(this);
    actShowAll = new QAction(this);
    actHideAll->setText("Hide All");
    actShowAll->setText("Show All");
    cMenu->addAction(actHideAll);
    cMenu->addAction(actShowAll);


    connect(cMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(triggeredContext(QAction*)) );
}

// ----------------------------------- show ------------------------------------
void PlotterWid::show(const QByteArray &data, QColor color)
{
    Plot plot;
    plot.color = color;
    plot.width = param.plotWidth;
    plot.penStyle = param.plotPenStyle;


    QPixmap pm(20, 20);
    pm.fill(color);
    QAction* act = new QAction(this);
    act->setCheckable(true);
    act->setChecked(true);
    act->setIcon(QIcon(pm));
    cMenu->addAction(act);
    plot.act = act;


    plotList.push_back(plot);
    dataList.push_back(data);

    recalc(data, plotList.size() - 1);

    if(!isShow)
        emit showWidget(this);

    isShow = true;
}

// ----------------------------------- redraw ------------------------------------
void PlotterWid::redraw(const QByteArray &data, QColor color)
{
    int i = 0;
    // Находим индекс элемента, который нужно обновить
    for(auto plot : plotList)
    {
        if(plot.color == color)
            break;
        i++;
    }

    // Обновляем данные
    dataList[i] = data;

    // Пересчитываем все точки всех графиков
    recalcAll();
    repaint();
}

// --------------------------------- removePlot ------------------------------------
void PlotterWid::removePlot(QColor color)
{

    int i = 0;
    // Находим индекс элемента, который нужно удалить
    for(auto plot : plotList)
    {
        if(plot.color == color)
            break;
        i++;
    }
    plotList.removeAt(i);
    dataList.removeAt(i);

    // Обновляем графики на всякий
    recalcAll();
    repaint();
}

// ----------------------------------- setParam ------------------------------------
void PlotterWid::setParam(const Param &param)
{
    this->param = param;
    for(auto &plot : plotList)
    {
        plot.width = param.plotWidth;
    }
    recalcAll();
    repaint();
}

// ----------------------------------- getParam_sl ------------------------------------
void PlotterWid::getParam_sl()
{
    emit getParam_sg(param);
}

// ----------------------------------- paintEvent ------------------------------------
void PlotterWid::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    drawGraph(painter);
    drawGrid(painter);

}

// ----------------------------------- drawGrid ------------------------------------
void PlotterWid::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(QBrush(param.xGrid.colorLine), param.xGrid.widthLine,
                        param.xGrid.penStyle));
    painter.setFont(param.xGrid.textFont);
    qreal dx = static_cast<qreal>(param.rectGraph.width()) / param.xGrid.steps;

    qreal val = param.xGrid.beginVal;
    qreal dVal = static_cast<qreal>(param.xGrid.endVal - param.xGrid.beginVal)
            / param.xGrid.steps;
    if(isPercent)
    {
        dVal /= 2.55;
    }

    for(qreal x = param.rectGraph.left(); x < param.rectGraph.right() + dx; x += dx, val += dVal)
    {
        painter.setPen(QPen(QBrush(param.xGrid.colorLine), param.xGrid.widthLine,
                            param.xGrid.penStyle));

        painter.drawLine(static_cast<int>(x), param.rectGraph.top(),
                         static_cast<int>(x), param.rectGraph.bottom());

        painter.setPen(QPen(QBrush(param.xGrid.colorText), param.xGrid.widthLine,
                            param.xGrid.penStyle));
        QRect r(static_cast<int>(x - dx / 2), param.rectGraph.bottom(),
                static_cast<int>(dx), param.margin.bottom);
        painter.drawText(r, Qt::AlignmentFlag::AlignCenter,
                         QString::number(static_cast<int>(val)) + (isPercent ? "%" : ""));
    }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    painter.setPen(QPen(QBrush(param.yGrid.colorLine), param.yGrid.widthLine,
                        param.yGrid.penStyle));
    painter.setFont(param.yGrid.textFont);

    val = param.yGrid.beginVal;
    dVal = static_cast<qreal>(param.yGrid.endVal - param.yGrid.beginVal) / param.yGrid.steps;

    if(isPercent)
    {
        dVal /= 2.55;
    }

    qreal dy = static_cast<qreal>(param.rectGraph.height()) / param.yGrid.steps;
    for(qreal y = param.rectGraph.bottom();  y > param.rectGraph.top() - dy; y -= dy, val += dVal)
    {
        painter.setPen(QPen(QBrush(param.yGrid.colorLine), param.yGrid.widthLine,
                            param.yGrid.penStyle));
        painter.drawLine(param.rectGraph.left(),  static_cast<int>(y),
                         param.rectGraph.right(), static_cast<int>(y));

        painter.setPen(QPen(QBrush(param.yGrid.colorText), param.yGrid.widthLine,
                            param.yGrid.penStyle));
        QRect r(param.rectGraph.left()-param.margin.left,static_cast<int>(y - dy / 2),
               param.margin.left, static_cast<int>(dy));
        painter.drawText(r, Qt::AlignmentFlag::AlignCenter,
                         QString::number(static_cast<int>(val))+ (isPercent ? "%" : ""));
    }

    // Рисуем оси:
    painter.setPen(QPen(QBrush(param.xAxe.color), param.xAxe.width,
                        param.xAxe.penStyle));
    painter.drawLine(param.rectGraph.bottomLeft(), param.rectGraph.topLeft());

    painter.setPen(QPen(QBrush(param.yAxe.color), param.yAxe.width,
                        param.yAxe.penStyle));
    painter.drawLine(param.rectGraph.bottomLeft(), param.rectGraph.bottomRight());
}

// ----------------------------------- drawGraph ------------------------------------
void PlotterWid::drawGraph(QPainter& painter)
{
    for(auto plot : plotList)
    {
        if(!plot.act->isChecked())
            continue;

        if(plot.points.isEmpty())
            continue;
        painter.setPen(QPen(QBrush(plot.color), plot.width, plot.penStyle));

        auto lastPoint = plot.points.first();
        for(auto point : plot.points)
        {
            painter.drawLine(lastPoint, point);
            lastPoint = point;
        }
    }
}

// ----------------------------------- recalc ------------------------------------
void PlotterWid::recalc(const QByteArray &data, int i)
{
    QPointF p0(param.rectGraph.left(), param.rectGraph.bottom());
    param.xGrid.endVal = param.xGrid.endVal < data.size() ? data.size() : param.xGrid.endVal;
    qreal x = 0;
    qreal dx = static_cast<qreal>(param.rectGraph.width()) / (param.xGrid.endVal - param.xGrid.beginVal);
    qreal dy = static_cast<qreal>(param.rectGraph.height()) / 255;
    plotList[i].points.clear();

    for(auto itD = data.begin(); itD < data.end(); itD++, x += dx)
    {
        plotList[i].points.push_back(p0 + QPointF(x, -static_cast<uint8_t>(*itD)*dy));
    }
}

// ------------------------------------ recalcAll -------------------------------------
void PlotterWid::recalcAll()
{
    int i = 0;
    param.xGrid.endVal = 0;
    for(auto data : dataList)
    {
        if(!plotList.at(i++).act->isChecked())
        {
            continue;
        }
        if(param.xGrid.endVal < data.size())
        {
            param.xGrid.endVal = data.size();
        }
    }

    i = 0;
    for(auto data : dataList)
    {
        if(plotList.at(i).act->isChecked())
        {
            recalc(data, i++);
        }
    }

}

// ----------------------------------- triggeredContext --------------------------------
void PlotterWid::triggeredContext(QAction *act)
{
    if(act == actHideAll)
    {
        for (auto plot : plotList)
        {
            plot.act->setChecked(false);
        }
    }
    else if (act == actShowAll)
    {
        for (auto plot : plotList)
        {
            plot.act->setChecked(true);
        }
    }
    recalcAll();
    repaint();
}

// ----------------------------------- resizeEvent ------------------------------------
void PlotterWid::resizeEvent(QResizeEvent *event)
{
    param.rectGraph = QRect(param.margin.left, param.margin.top,
                       event->size().width() - param.margin.left-param.margin.right,
                       event->size().height() - param.margin.top-param.margin.bottom);

    //param.yGrid.steps = param.rectGraph.height() / param.yGrid.widthStep;
    recalcAll();
}

// ----------------------------------- closeEvent ------------------------------------
void PlotterWid::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    emit closed();
}

// ----------------------------------- changeEvent ------------------------------------
void PlotterWid::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::Type::WindowStateChange)
    {
        recalcAll();
    }
}

// ----------------------------------- plotImage ------------------------------------
QImage PlotterWid::plotImage()
{
    QPixmap pixmap(this->size());
    this->render(&pixmap);
    return pixmap.toImage();
}

// ----------------------------------- contextMenuEvent -----------------------------
void PlotterWid::contextMenuEvent(QContextMenuEvent *event)
{
    cMenu->exec(event->globalPos());
}

// ----------------------------- FabricPlotterWid ------------------------------
IPlotterWid *FactoryPlotterWid::create(QWidget *parent)
{
    return new PlotterWid(parent);
}
