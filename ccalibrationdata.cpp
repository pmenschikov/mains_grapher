#include <QFile>
#include <QTextStream>

#include "ccalibrationdata.h"

CCalibrationData::CCalibrationData():
  m_data(6)
{
        load_defaults();
}

float CCalibrationData::voltage_calib(int channel) const
{
  return m_data[channel];
}

float CCalibrationData::current_calib(int channel, bool gain) const
{
  Q_UNUSED(gain)
  int t = 0;//gain ? 3:0;
  return m_data[channel+t];
}

void CCalibrationData::load_defaults()
{
 m_data[0] = 0.0732f;
 m_data[1] = 0.0004883f;
 m_data[2] = 0.0732f;
 m_data[3] = 0.0004883f;
 m_data[4] = 0.0732f;
 m_data[5] = 0.0004883f;
}

bool CCalibrationData::load_from_file(QString filename)
{
  QFile f;
  f.setFileName(filename);

  if( f.open(QIODevice::ReadOnly))
    {
      QTextStream s(&f);
      for(int i=0; i<m_data.length(); i++)
        {
          QString line = s.readLine();
          bool conv;
          float t = line.toFloat(&conv);
          if( !conv )
            return false;

          m_data[i] = t;
        }
    }

  return true;
}

void CCalibrationData::save_to_file(QString filename)
{
  QFile f;
  f.setFileName(filename);

  if( f.open(QIODevice::WriteOnly))
    {
      QTextStream s(&f);
      for(int i=0; i<m_data.length(); i++)
        {
          QString st(QString("%1").arg(m_data[i],0,'f',7));
          s << st << "\n";
        }
    }
}
