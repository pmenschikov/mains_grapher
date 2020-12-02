#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "ccalibrationdata.h"

CCalibrationData::CCalibrationData():
  m_data(6),
  m_offsets(9)
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

 m_offsets =  {165,111,170,310,192,303,1855,666,800};
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
      for(int i=0; i<m_offsets.length(); i++)
        {
          QString line = s.readLine();
          bool conv;
          auto t = static_cast<int16_t>(line.toInt(&conv));
          if( !conv )
            return false;

          m_offsets[i] = t;
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
      for(int i=0; i<m_offsets.length(); i++)
        {
          QString st(QString("%1").arg(m_offsets[i]));
          s << st << "\n";
        }
    }
}
