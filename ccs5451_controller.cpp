#include <QDebug>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <QDataStream>

#include "cmeasurements.h"
#include "ccs5451_controller.h"
#include "cadcctrl_interface.h"

const char *g_calibration_file = "calibration.data";

CCS5451_controller::CCS5451_controller():
  m_gain(false),
  m_measurements(new CMeasurements(this)),
  m_offsets(9)
{
  m_offsets = {165,111,170,310,192,303,1855,666,800};

  if(!m_calibration.load_from_file(g_calibration_file))
    m_calibration.load_defaults();
}

CCS5451_controller::~CCS5451_controller()
{
  m_calibration.save_to_file(g_calibration_file);
  delete m_measurements;
}

void CCS5451_controller::start()
{
    qDebug() << __PRETTY_FUNCTION__;
    m_interface->send(const_cast<char*>("e3\na1\n"),6);
  sleep(1);
//    m_interface->send(const_cast<char*>("a1\n"),3);
}

void CCS5451_controller::stop()
{
 //   qDebug() << __PRETTY_FUNCTION__;
}

void CCS5451_controller::new_data(char *data,int len)
{
   auto t = QByteArray(data, len);

   auto adc = extract_data(t);
   translate(adc);
   put(adc);

}

adc_data CCS5451_controller::extract_data(QByteArray& data)
{
   adc_data adc_data;
   QDataStream s(&data, QIODevice::ReadOnly);
   s.setByteOrder(QDataStream::LittleEndian);
   s >> adc_data.data_num;
   s >> adc_data.voltage_a;
   s >> adc_data.current_a;
   s >> adc_data.voltage_b;
   s >> adc_data.current_b;
   s >> adc_data.voltage_c;
   s >> adc_data.current_c;

static int count = 0;
static QVector<long> sums(6,0);
count++;
	if( count == 160)
	  {
	   for(int i=0; i<6; i++)
	     sums[i]/= 160;
//	   qDebug() << "Raw Avg: " << sums;
	   sums.fill(0);
	   count=0;
	  }
	sums[0]+= adc_data.voltage_a;
	sums[1]+= adc_data.voltage_b;
	sums[2]+= adc_data.voltage_c;
	sums[3]+= adc_data.current_a;
	sums[4]+=adc_data.current_b;
	sums[5]+=adc_data.current_c;


   return adc_data;
}

void CCS5451_controller::translate(adc_data &data)
{
   data.voltage_a += m_offsets[0];
   data.voltage_b += m_offsets[1];
   data.voltage_c += m_offsets[2];
   if( !m_gain )
   {
     data.current_a += m_offsets[3];
     data.current_b += m_offsets[4];
     data.current_c += m_offsets[5];
   }
   else
   {
     data.current_a += m_offsets[6];
     data.current_b += m_offsets[7];
     data.current_c += m_offsets[8];
   }
}

double  CCS5451_controller::value(int channel)
{
    Q_UNUSED(channel)
    return 0.0;
}

int CCS5451_controller::raw_value(int channel)
{
    Q_UNUSED(channel)
    return 0;
}

bool CCS5451_controller::open()
{
    return true;
  //  return m_interface->open();
}

void CCS5451_controller::set_owrs(bool on)
{
    send_cmd(on ? "c0":"c1");
}

void CCS5451_controller::set_gain(bool on)
{
  m_gain = on;
    send_cmd( on ? "d0":"d1");
}

void CCS5451_controller::send_cmd(const char *cmd)
{
    if( m_interface->isOpen())
    {
        char cmd_buf[8];
        int len = snprintf(cmd_buf,8-1, "%s\n", cmd);
        len = m_interface->send(cmd_buf, len);
        //qDebug() << "send: " << len;
    }
}

void CCS5451_controller::error_slot(int err)
{
Q_UNUSED(err)
}

CMeasurements* CCS5451_controller::measurements()
{
  m_measurements->measure();
  return m_measurements;
}

float CCS5451_controller::rms_voltage(int channel)
{
  return m_measurements->m_rms[channel]*m_calibration.voltage_calib(channel);
}
float CCS5451_controller::rms_current(int channel)
{
  if( m_gain)
    return m_measurements->m_rms[channel]*m_calibration.current_calib(channel,m_gain)/20.0f;
  else
    return m_measurements->m_rms[channel]*m_calibration.current_calib(channel,m_gain);
}

float CCS5451_controller::p_power(int channel)
{
  float factor =
      m_calibration.voltage_calib(channel*2)*
      m_calibration.current_calib(channel*2+1, m_gain);
  if(m_gain)
  return m_measurements->m_active_powers[channel]*factor/20.0f;
  else
  return m_measurements->m_active_powers[channel]*factor;
}

float CCS5451_controller::s_power(int channel)
{
  float voltage = rms_voltage(channel*2);
  float current= rms_current(channel*2+1);
  return voltage*current;
}

float CCS5451_controller::pf(int channel)
{
  return p_power(channel)/s_power(channel);
}

QVector<int16_t> CCS5451_controller::zero()
{
  QVector<long> temp(6,0);

  const int meas_count = 4*40;
  for(int i=0; i<meas_count; i++)
    {
      for(int f=0; f<6; f++)
         temp[f]+=m_data[i].field_value(f);
    }
  QVector<int16_t> result(6);
  const int c_off = m_gain ? 6:3;

  result[0]=static_cast<int16_t>((temp[0]-meas_count*m_offsets[0])/meas_count);
  result[1]=static_cast<int16_t>((temp[1]-meas_count*m_offsets[c_off])/meas_count);
  result[2]=static_cast<int16_t>((temp[2]-meas_count*m_offsets[1])/meas_count);
  result[3]=static_cast<int16_t>((temp[3]-meas_count*m_offsets[c_off+1])/meas_count);
  result[4]=static_cast<int16_t>((temp[4]-meas_count*m_offsets[2])/meas_count);
  result[5]=static_cast<int16_t>((temp[5]-meas_count*m_offsets[c_off+2])/meas_count);

  qDebug()<<"res:"<<result;
  qDebug()<<m_offsets;
  m_offsets[0] = -result[0];
  m_offsets[1] = -result[2];
  m_offsets[2] = -result[4];

  m_offsets[c_off] = -result[1];
  m_offsets[c_off+1] = -result[3];
  m_offsets[c_off+2] = -result[5];
  qDebug()<<m_offsets;
  return result;
}

CCalibrationData& CCS5451_controller::get_calibration()
{
  return m_calibration;
}

void CCS5451_controller::set_calibration(CCalibrationData data)
{
  m_calibration = data;
}
