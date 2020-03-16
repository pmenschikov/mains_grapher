#ifndef PLOT_H
#define PLOT_H

#include <qwt_plot.h>

class QwtPlotDirectPainter;
class QwtPlotMarker;
class QwtPlotCurve;

class BufferedCurve;

class Plot : public QwtPlot
{
    Q_OBJECT

public:
    Plot( QWidget*,
          BufferedCurve *a,
          BufferedCurve *b,
          BufferedCurve *c);

    virtual ~Plot();

    void start();
    virtual void replot();
    virtual bool eventFilter(QObject *, QEvent*);

protected:
    virtual void showEvent(QShowEvent*);
    virtual void resizeEvent(QResizeEvent*);
    virtual void timerEvent(QTimerEvent *){}

private:
    QwtPlotDirectPainter *painter;
    QwtPlotMarker *d_origin;
    QwtPlotCurve *d_curve;
};

#endif // PLOT_H
