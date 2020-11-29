#include "cdatabufffer.h"
#include <QDebug>
#include <QDataStream>
#include <QStringBuilder>

#include "bufferedcurve.h"

adc_data::adc_data(QByteArray data)
{
   QDataStream s(&data, QIODevice::ReadOnly);
   s.setByteOrder(QDataStream::LittleEndian);
   s >> data_num;
   s >> voltage_a;
   s >> current_a;
   s >> voltage_b;
   s >> current_b;
   s >> voltage_c;
   s >> current_c;
   voltage_a+=165;
   voltage_b+=111;
   voltage_c+=170;
   current_a+=310;
   current_b+=192;
   current_c+=303;
}

QString adc_data::toStr() const
{
    QString ret;
    QTextStream s(&ret);

    s << "(" << Qt::hex << data_num;
    s << "," << voltage_a;
    s << "," << current_a;
    s << "," << voltage_b;
    s << "," << current_b;
    s << "," << voltage_c;
    s << "," << current_c;
    s << ")";
    return ret;
}

void CDataBuffer::init()
{
    m_data.resize(m_capacity);
    m_apower_sum.resize(3);
    //qDebug() << "capacity: " << m_data.capacity() << " size: " << m_data.size();
}

void CDataBuffer::put(QByteArray data)
{
  put(adc_data(data));
}

void CDataBuffer::put(adc_data data)
{
    m_data[m_current] = data;
    m_current++;
    if( m_current >= m_capacity)
    {
        m_current = 0;
    }
}

QVector<adc_data> CDataBuffer::get(int count)
{
    QVector<adc_data> ret;

    if( m_current > count )
    {
        ret = m_data.mid(m_current, count);
    }

    return ret;
}

int16_t CDataBuffer::get_value(int pos, int field)
{
//    qDebug() << pos << ":" << m_data[pos].voltage_a;
    int position;
    if( (m_snapshot>=40*4))
        position = m_snapshot-40*4+pos;
    else
    {
  //      qDebug() << m_capacity << "," << m_snapshot << "," << pos;
        position = (m_capacity+m_snapshot-40*4+pos)%m_capacity;
    }
   // qDebug() << "position: " << position;
   return m_data[position].field_value(field);
}

BufferedCurve* CDataBuffer::get_curve_data(int field)
{
    BufferedCurve *curve = new BufferedCurve(this, field, 40*4);
    m_curves.push_back(curve);

    return curve;
}

void CDataBuffer::prepare()
{
    BufferedCurve *curve;
    m_snapshot = m_current;
    foreach(curve, m_curves)
    {
        curve->calc_bounds();
    }
}








