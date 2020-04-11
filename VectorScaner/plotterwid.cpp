#include "plotterwid.h"
using namespace _VectorScan;

PlotterWid::PlotterWid(QWidget *parent, IParametres* par) :
    QWidget(parent)
{
    // Начальный размер просто от балды)
    this->resize(650, 400);

    this->setMinimumSize(325, 200);
    this->setWindowFlag(Qt::WindowType::WindowStaysOnTopHint, true);

    cMenu = new QMenu(this);

    actHideAll = new QAction(this);
    actShowAll = new QAction(this);
    actHideAll->setText("Hide All");
    actShowAll->setText("Show All");
    cMenu->addAction(actHideAll);
    cMenu->addAction(actShowAll);

    connect(cMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(triggeredContext(QAction*)) );


    connect(par, SIGNAL(updateParam(IParamData*)),
            this, SLOT(updateParam(IParamData*)) );
}

// ----------------------------------- show ------------------------------------
void PlotterWid::show(const QByteArray &data, QColor color, int id)
{
    Plot plot;
    plot.color = color;
    plot.width = param->plotWidth();
    plot.id = id;

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
void PlotterWid::redraw(const QByteArray &data, int id)
{
    int i = 0;
    // Находим индекс элемента, который нужно обновить
    for(auto plot : plotList)
    {
        if(plot.id == id)
            break;
        i++;
    }

    if(i == plotList.size())
    {
        qDebug() << "Index of out!";
        return;
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
    cMenu->removeAction(plotList.at(i).act);
    plotList.removeAt(i);
    dataList.removeAt(i);

    // Обновляем графики на всякий
    recalcAll();
    repaint();
}

// ----------------------------------- paintEvent ------------------------------------
void PlotterWid::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    drawGraph(painter);
    drawGrid(painter);
}

// ----------------------------------- updateParam ------------------------------------
void PlotterWid::updateParam(IParamData * data)
{
    auto temp = dynamic_cast<ParamPlotterWid*>(data);
    if(temp)
    {
        if(param)
        {
            for(auto &plot : plotList)
            {
                plot.width = param->plotWidth();
            }
            if(isShow)
            {
                recalcAll();
                repaint();
            }
        }
        else
        {
            param = temp;
        }
        rectGraph = QRect(param->margin().left, param->margin().top,
                           this->size().width() - param->margin().left-param->margin().right,
                           this->size().height() - param->margin().top-param->margin().bottom);

    }
}

// ----------------------------------- drawGrid ------------------------------------
void PlotterWid::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(QBrush(param->xGrid().colorLine), param->xGrid().widthLine,
                        param->xGrid().penStyle));
    painter.setFont(param->xGrid().textFont);
    qreal dx = static_cast<qreal>(rectGraph.width()) / param->xGrid().steps;

    qreal val = param->xGrid().beginVal;
    qreal dVal = static_cast<qreal>(param->xGrid().endVal - param->xGrid().beginVal)
            / param->xGrid().steps;

    for(qreal x = rectGraph.left(); x < rectGraph.right() + dx; x += dx, val += dVal)
    {
        painter.setPen(QPen(QBrush(param->xGrid().colorLine), param->xGrid().widthLine,
                            param->xGrid().penStyle));

        painter.drawLine(static_cast<int>(x), rectGraph.top(),
                         static_cast<int>(x), rectGraph.bottom());

        painter.setPen(QPen(QBrush(param->xGrid().colorText), param->xGrid().widthLine,
                            param->xGrid().penStyle));
        QRect r(static_cast<int>(x - dx / 2), rectGraph.bottom(),
                static_cast<int>(dx), param->margin().bottom);
        painter.drawText(r, Qt::AlignmentFlag::AlignCenter,
                         QString::number(static_cast<int>(std::trunc(val))));
    }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    painter.setPen(QPen(QBrush(param->yGrid().colorLine), param->yGrid().widthLine,
                        param->yGrid().penStyle));
    painter.setFont(param->yGrid().textFont);

    val = param->yGrid().beginVal;
    dVal = static_cast<qreal>(param->yGrid().endVal - param->yGrid().beginVal) / param->yGrid().steps;


    if(param->yMode() == ParamPlotterWid::Percent)
    {
        dVal /= 2.55;
    }

    qreal dy = static_cast<qreal>(rectGraph.height()) / param->yGrid().steps;
    for(qreal y = rectGraph.bottom();  y > rectGraph.top() - dy; y -= dy, val += dVal)
    {
        painter.setPen(QPen(QBrush(param->yGrid().colorLine), param->yGrid().widthLine,
                            param->yGrid().penStyle));
        painter.drawLine(rectGraph.left(),  static_cast<int>(y),
                         rectGraph.right(), static_cast<int>(y));

        painter.setPen(QPen(QBrush(param->yGrid().colorText), param->yGrid().widthLine,
                            param->yGrid().penStyle));
        QRect r(rectGraph.left()-param->margin().left,static_cast<int>(y - dy / 2),
               param->margin().left, static_cast<int>(dy));
        painter.drawText(r, Qt::AlignmentFlag::AlignCenter,
                         QString::number(static_cast<int>(std::trunc(val))) +
                         (param->yMode() == ParamPlotterWid::Percent ? "%" : ""));
    }

    // Рисуем оси:
    painter.setPen(QPen(QBrush(param->xAxe().color), param->xAxe().width,
                        param->xAxe().penStyle));
    painter.drawLine(rectGraph.bottomLeft(), rectGraph.topLeft());

    painter.setPen(QPen(QBrush(param->yAxe().color), param->yAxe().width,
                        param->yAxe().penStyle));
    painter.drawLine(rectGraph.bottomLeft(), rectGraph.bottomRight());
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
        painter.setPen(QPen(QBrush(plot.color), plot.width, param->plotPenStyle()));

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
    QPointF p0(rectGraph.left(), rectGraph.bottom());
    // Ну эт конечно костыль) Надо бы как-то по-другому сделать
    const_cast<ParamPlotterWid::Grid&>(param->xGrid()).endVal = param->xGrid().endVal < data.size() ? data.size() : param->xGrid().endVal;
    qreal x = 0;
    qreal dx = static_cast<qreal>(rectGraph.width()) / (param->xGrid().endVal - param->xGrid().beginVal);
    qreal dy = static_cast<qreal>(rectGraph.height()) / 255;
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
    const_cast<ParamPlotterWid::Grid&>(param->xGrid()).endVal = 0;
    for(auto data : dataList)
    {
        if(!plotList.at(i++).act->isChecked())
        {
            continue;
        }
        if(param->xGrid().endVal < data.size())
        {
            const_cast<ParamPlotterWid::Grid&>(param->xGrid()).endVal = data.size();
        }
    }

    i = 0;
    for(auto data : dataList)
    {
        if(plotList.at(i++).act->isChecked())
        {
            recalc(data, i-1);
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
    rectGraph = QRect(param->margin().left, param->margin().top,
                       event->size().width() - param->margin().left-param->margin().right,
                       event->size().height() - param->margin().top-param->margin().bottom);

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
