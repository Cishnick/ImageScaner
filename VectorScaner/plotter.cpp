#include "plotter.h"
#include "math.h"

using namespace _VectorScan;

// --------------------------------- CaclLine ------------------------------------
Plotter::Plotter(QObject *parent, IParametres *par) : IPlotter(parent)
{
    plot = new PlotterWid(nullptr, par);

    connect(par, SIGNAL(updateParam(IParamData*)),
            this, SLOT(updateParam(IParamData*)) );

    connect(this, SIGNAL(show(QByteArray const&, QColor, int)),
            plot, SLOT(show(QByteArray const&, QColor, int)) );

    connect(this, SIGNAL(redraw(QByteArray const&, int)),
            plot, SLOT(redraw(QByteArray const&, int)) );

    connect(plot, SIGNAL(showWidget(QWidget*)),
            this, SIGNAL(showWidget(QWidget*)) );

    connect(plot, SIGNAL(closed()),
            this, SIGNAL(closed()) );
}

// --------------------------------- ~CalcLine ------------------------------------
Plotter::~Plotter()
{
    if(plot)
        delete plot;
}

// --------------------------------- PostProcessing ------------------------------------
QByteArray Plotter::PostProcessing(QByteArray &&data)
{
    return std::move(data);
}

// --------------------------------- BresenhemAlg ------------------------------------
QByteArray Plotter::BresenhamAlg()
{
    QByteArray res;
    int st = param->step();
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
            if(st == param->step())
            {
                res.push_back(static_cast<char>(
                                  img.data.at(
                                      static_cast<size_t>(y + y0)*img.width +
                                      static_cast<size_t>(x + x0)                                      )
                                  ));
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
            if(st == param->step())
            {
                res.push_back(static_cast<char>(
                                  img.data.at(
                                      static_cast<size_t>(y + y0)*img.width +
                                      static_cast<size_t>(x + x0)                                      )
                                  ));
                st = 0;
            }
            st++;
        }while((x != X) || (y != Y));
    }
    return res;
}

// --------------------------------- vectorPainted ------------------------------------
void Plotter::vectorPainted(const Vector &vect)
{
    vector = vect;
    emit show(BresenhamAlg(), vector.color, vect.id);
}

// --------------------------------- vectorChanged ------------------------------------
void Plotter::vectorChanged(const Vector &vect)
{
    vector = vect;
    emit redraw(PostProcessing(BresenhamAlg()), vector.id);
}

// --------------------------------- openedByteImage ------------------------------------
void Plotter::openedByteImage(const image_t &image, QString const& filename)
{
    Q_UNUSED(image)
    plot->setWindowTitle("График "+filename);
    img = image;
}

// --------------------------------- vectorRemove ------------------------------------
void Plotter::vectorRemove(QColor color)
{
    plot->removePlot(color);
}

// --------------------------------- plotImage ------------------------------------
QImage Plotter::plotImage()
{
    return plot->plotImage();
}

// --------------------------------- plotImage ------------------------------------
void Plotter::updateParam(IParamData * data)
{
    auto temp = dynamic_cast<ParamPlotter*>(data);
    if(temp)
    {
        if(param)
        {
            vectorChanged(this->vector);
        }
        else
        {
            param = temp;
        }
    }
}

// --------------------------- FabricPlotter --------------------------------------------
IPlotter* FactoryPlotter::create(QObject *parent, IParametres* par)
{
    return new Plotter(parent, par);
}
