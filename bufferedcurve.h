#ifndef BUFFEREDCURVE_H
#define BUFFEREDCURVE_H

#include <qwt_series_data.h>

class CDataBuffer;

class BufferedCurve : public QwtSeriesData<QPointF>
{
public:
    BufferedCurve(CDataBuffer*,int, size_t);

    virtual QPointF sample(size_t i) const;
    virtual size_t size() const;
    virtual QRectF boundingRect() const;

    void calc_bounds();

private:
    CDataBuffer *m_databuffer;
    int m_fieldnum;
    size_t m_size;
    QRectF m_boundingrect;
};


#endif // BUFFEREDCURVE_H
