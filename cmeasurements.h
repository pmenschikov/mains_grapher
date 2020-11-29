#ifndef CMEASUREMENTS_H
#define CMEASUREMENTS_H

#include "cdatabufffer.h"
class CMeasurements
{
public:
  explicit CMeasurements(CDataBuffer *);

  void measure();

  float rms_voltage(int channel);
  float rms_current(int channel);

public:
  QVector<int16_t> m_mins;
  QVector<int16_t> m_maxs;
  QVector<float> m_avg;
  QVector<float> m_rms;
  QVector<float> m_active_powers;
protected:
  CDataBuffer *m_ctrl;
};

#endif // CMEASUREMENTS_H
