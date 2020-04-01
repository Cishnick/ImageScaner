#include "calcline.h"
#include "math.h"

using namespace _VectorScan;

// --------------------------------- CaclLine ------------------------------------
CalcLine::CalcLine(QObject *parent) : IPlotter(parent)
{
    plot = FactoryPlotterWid::create(nullptr);

    connect(this, SIGNAL(show(QByteArray const&, QColor)),
            plot, SLOT(show(QByteArray const&, QColor)) );

    connect(this, SIGNAL(redraw(QByteArray const&, QColor)),
            plot, SLOT(redraw(QByteArray const&, QColor)) );

    connect(plot, SIGNAL(showWidget(QWidget*)),
            this, SIGNAL(showWidget(QWidget*)) );

    connect(plot, SIGNAL(closed()),
            this, SIGNAL(closed()) );
}

// --------------------------------- ~CalcLine ------------------------------------
CalcLine::~CalcLine()
{
    if(plot)
        delete plot;
}

// --------------------------------- BresenhemAlg ------------------------------------
QByteArray CalcLine::BresenhamAlg()
{
    QByteArray res;
    int st = step;
    int x0 = static_cast<int>(vector.begin.x());
    int y0 = static_cast<int>(vector.begin.y());
    int x1 = static_cast<int>(vector.end.x());
    int y1 = static_cast<int>(vector.end.y());

    int X = x1 - x0;
    int Y = y1 - y0;

    if((X == 0) && (Y == 0))
        return QByteArray();

    int x = 0, y = 0;
    float tg = static_cast<float>(Y) / X;
    float f = 0.f;
    int dx = (X > 0) ? 1 : -1;
    int dy = (Y > 0) ? 1 : -1;
    if(abs(Y) < abs(X))
    {
        do
        {
            x += dx;
            f = x*tg;
            if(fabs(f - y) > 0.5f)
            {
                y += dy;
            }
            if(st == step)
            {
                res.push_back(img.data.at((x + x0)*img.size.height() + (y + y0)));
                st = 0;
            }
            st++;
        }while((x != X) || (y != Y));
    }
    else
    {
        do
        {
            y+=dy;
            f = y/tg;
            if(fabs(f - x) > 0.5f)
            {
                x+=dx;
            }
            if(st == step)
            {
                res.push_back(img.data.at((x + x0)*img.size.height() + (y + y0)));
                st = 0;
            }
            st++;
        }while((x != X) || (y != Y));
    }
    return res;
}

// --------------------------------- vectorPainted ------------------------------------
void CalcLine::vectorPainted(const Vector &vect)
{
    vector = vect;
    emit show(BresenhamAlg(), vector.color);
}

// --------------------------------- vectorChanged ------------------------------------
void CalcLine::vectorChanged(const Vector &vect)
{
    vector = vect;
    emit redraw(BresenhamAlg(), vector.color);
}

// --------------------------------- openedByteImage ------------------------------------
void CalcLine::openedByteImage(const ByteImage &image, QString const& filename)
{
    plot->setWindowTitle("Plot of "+filename);
    img = image;
}

// --------------------------------- vectorRemove ------------------------------------
void CalcLine::vectorRemove(QColor color)
{
    plot->removePlot(color);
}

// --------------------------------- plotImage ------------------------------------
QImage CalcLine::plotImage()
{
    return plot->plotImage();
}

// --------------------------- FabricPlotter --------------------------------------------
IPlotter* FactoryPlotter::create(QObject *parent)
{
    return new CalcLine(parent);
}
