#include <QVector>
#include <cmath>

#include "cmeasurements.h"

CMeasurements::CMeasurements(CDataBuffer *ctrl):
  m_mins(6), m_maxs(6), m_rms(6),m_active_powers(3),
  m_ctrl(ctrl)
{

}

void CMeasurements::measure()
{
  const int meas_count = 40*4;

  QVector<long> sums(6,0);
  QVector<long> sums_rms(6,0);
  QVector<long> active_p_sums(3,0);
  m_mins.fill(INT16_MAX);
  m_maxs.fill(INT16_MIN);

  for(int i=0; i<meas_count; i++)
  {
    for(int ch=0; ch<6; ch++)
    {
        int16_t value = m_ctrl->get_value(i,ch);
        sums[ch] += value;
        sums_rms[ch] += value*value;
        m_mins[ch] = qMin(m_mins[ch], value);
        m_maxs[ch] = qMax(m_maxs[ch],value);
    }
    for(int j=0; j<3; j++)
      {
        int voltage = m_ctrl->get_value(i,j*2);
        int current = m_ctrl->get_value(i,j*2+1);
        active_p_sums[j] += voltage*current;
      }
  }

  for(int i=0; i<3; i++)
  {
    m_rms[i*2] = std::sqrt(static_cast<float>(sums_rms[i*2])/meas_count);
    m_rms[i*2+1] = std::sqrt(static_cast<float>(sums_rms[i*2+1])/meas_count);
    m_active_powers[i] = static_cast<float>(active_p_sums[i])/meas_count;
  }
}
