#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_directpainter.h>
#include <qwt_curve_fitter.h>
#include <qwt_painter.h>
#include <qwt_scale_engine.h>
#include <qevent.h>

#include "plot.h"
#include "bufferedcurve.h"

class CurveData: public QwtSeriesData<QPointF>
{
public:
    CurveData(qreal phase=0.0):
        m_phase(phase)
    {}
    virtual QPointF sample(size_t i) const
    {
        qreal x = (qreal)(i)/(COUNT-1)*2*M_PI;
        qreal y = sin(x+m_phase);
        return QPointF(x,y);
    }

    virtual size_t size() const
    {
        return COUNT;
    }

    virtual QRectF boundingRect() const
    {
        return QRectF(0,-1, 2*M_PI ,2);
    }

    const size_t COUNT = 100;
    qreal m_phase;
};

Plot::Plot(QWidget *parent,
           BufferedCurve *a,
           BufferedCurve *b,
           BufferedCurve *c):
    QwtPlot(parent)
{
    setAutoReplot(false);
    setCanvas(new QwtPlotCanvas());

//    setAxisTitle( QwtPlot::xBottom, "X");
    setAxisScale(QwtPlot::xBottom, 0.0,1.0);
    setAxisAutoScale(QwtPlot::yLeft, true);
    //setAxisScale(QwtPlot::yLeft, -32000, 32000);
    //setAxisScale(QwtPlot::yLeft, -320, 320);
    //setAxisAutoScale(QwtPlot::yLeft);

    axisScaleEngine(QwtPlot::yLeft)->setAttribute(QwtScaleEngine::Floating,true);
    axisScaleEngine(QwtPlot::yRight)->setAttribute(QwtScaleEngine::Floating,true);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(Qt::gray);
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(false);
    grid->attach(this);

#if 1
    d_origin= new QwtPlotMarker();
    d_origin->setLineStyle(QwtPlotMarker::Cross);
    d_origin->setValue(0, 1);
    d_origin->setLinePen(Qt::gray, 0.0, Qt::DashLine);
    d_origin->attach(this);
#endif

    d_curve = new QwtPlotCurve();
    d_curve->setData(a);
    d_curve->setPen(Qt::yellow,4.0);
    d_curve->attach(this);
    d_curve->setAxes(QwtPlot::xBottom, QwtPlot::yLeft);

    d_curve = new QwtPlotCurve();
    d_curve->setData(b);
    d_curve->setPen(Qt::green,4.0);
    d_curve->attach(this);
    d_curve->setAxes(QwtPlot::xBottom, QwtPlot::yLeft);

    d_curve = new QwtPlotCurve();
    d_curve->setData(c);
    d_curve->setPen(Qt::red,4.0);
    d_curve->attach(this);
    d_curve->setAxes(QwtPlot::xBottom, QwtPlot::yLeft);
}

Plot::~Plot()
{

}
void Plot::replot()
{

    QwtPlot::replot();
}
void Plot::resizeEvent(QResizeEvent *event)
{
    QwtPlot::resizeEvent(event);
}

bool Plot::eventFilter(QObject *object, QEvent *event)
{
    return QwtPlot::eventFilter(object, event);
}

void Plot::showEvent(QShowEvent *)
{

}
