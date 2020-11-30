#ifndef CCALIBRATIONDATA_H
#define CCALIBRATIONDATA_H

#include <QVector>

class CCalibrationData
{
public:
  CCalibrationData();

  float voltage_calib(int channel) const;
  float current_calib(int channel, bool gain) const;

  void save_to_file(QString filename);
  bool load_from_file(QString filename);

  void load_defaults();

public:
  QVector<float> m_data;
};

#endif // CCALIBRATIONDATA_H
