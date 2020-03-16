#ifndef CDATABUFFFER_H
#define CDATABUFFFER_H
#include <stdint.h>
#include <QVector>
#include <QList>
#include <QByteArray>

struct adc_data
{
    uint32_t data_num;
    int16_t voltage_a;
    int16_t current_a;
    int16_t voltage_b;
    int16_t current_b;
    int16_t voltage_c;
    int16_t current_c;

    explicit adc_data(QByteArray data);
    QString toStr() const;
    adc_data():
        voltage_a(0),
        current_a(0),
        voltage_b(0),
        current_b(0),
        voltage_c(0),
        current_c(0)
    {}

    int16_t field_value(int field)
    {
        switch(field)
        {
        case 0: return voltage_a;
        case 2: return voltage_b;
        case 4: return voltage_c;
        case 1: return current_a;
        case 3: return current_b;
        case 5: return current_c;

        }
        return 0;
    }
};

class BufferedCurve;

class CDataBuffer
{
public:
    CDataBuffer():
        m_capacity(40*50*2),
        m_current(0),
        m_snapshot(0)
    {init();}
    explicit CDataBuffer(int capacity):
        m_capacity(capacity),
        m_current(0),
        m_snapshot(0)
    {init();}

public:
    void put(QByteArray data);

    QVector<adc_data> get(int count);

    int16_t get_value(int, int);

    BufferedCurve *get_curve_data(int);

    void prepare();

    typedef QVector<int16_t> rms_t;
    rms_t rms();

private:
    void init();
protected:
    int m_capacity;
    int m_current;
    QVector<adc_data> m_data;
    QList<BufferedCurve*> m_curves;

    int m_snapshot;
};

#endif // CDATABUFFFER_H
