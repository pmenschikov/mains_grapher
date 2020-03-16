#include "bufferedcurve.h"
#include "cdatabufffer.h"

BufferedCurve::BufferedCurve(CDataBuffer* dbuffer, int field_num,
                             size_t size):
    m_databuffer(dbuffer),
    m_fieldnum(field_num),
    m_size(size),
    m_boundingrect(-1,-1,-2,-2)
{

}

size_t BufferedCurve::size() const
{
    return m_size;
}

QPointF BufferedCurve::sample(size_t i) const
{
    qreal value =  m_databuffer->get_value(i, m_fieldnum);
    return QPointF((qreal)(i)/(qreal)(m_size-1),value);
}

QRectF BufferedCurve::boundingRect() const
{
    return m_boundingrect;
}

void BufferedCurve::calc_bounds()
{
    qreal value = m_databuffer->get_value(0, m_fieldnum);
    m_boundingrect.setRect(0.0, value, 0.0, 0.0 );
    for(size_t i=1; i<m_size; i++)
    {
        qreal value = m_databuffer->get_value(i, m_fieldnum);
        if( m_boundingrect.top() > value)
        {
            m_boundingrect.setTop(value);
        }
        if( m_boundingrect.bottom() < value )
        {
            m_boundingrect.setBottom(value);
        }
    }

}
