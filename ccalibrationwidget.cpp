#include "ccalibrationwidget.h"
#include "ui_ccalibrationwidget.h"

CCalibrationWidget::CCalibrationWidget(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CCalibrationWidget)
{
  ui->setupUi(this);
  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      {
        ui->tblCalibration->setItem(i,j,new QTableWidgetItem);
        ui->tblOffsets->setItem(i,j,new QTableWidgetItem);
      }
}

CCalibrationWidget::~CCalibrationWidget()
{
  delete ui;
}

void CCalibrationWidget::set_calibration(const CCalibrationData &data)
{

  for(int i=0; i<3; i++)
    {
        ui->tblCalibration->item(0,i)->setText(
              QString("%1").arg(data.voltage_calib(i*2),0,'f',7));
        ui->tblCalibration->item(1,i)->setText(
              QString("%1").arg(data.current_calib(i*2+1,false), 0,'f',7));
        ui->tblCalibration->item(2,i)->setText(
              QString("%1").arg(data.current_calib(i*2+1,true), 0,'f',7));

        ui->tblOffsets->item(0,i)->setText(
              QString("%1").arg(data.m_offsets[i]));
        ui->tblOffsets->item(1,i)->setText(
              QString("%1").arg(data.m_offsets[i+3]));
        ui->tblOffsets->item(2,i)->setText(
              QString("%1").arg(data.m_offsets[i+6]));
    }
}

bool CCalibrationWidget::get_calibration(CCalibrationData &data)
{
  bool ret = true;

  auto tw = ui->tblCalibration;
  bool conv;

  for(int i=0; i<3; i++)
    {
      data.m_data[i*2] = tw->item(0,i)->text().toFloat(&conv);
      ret &= conv;
      data.m_data[i*2+1] = tw->item(1,i)->text().toFloat(&conv);
      ret &= conv;

      for(int j=0; j<3; j++)
        {
          data.m_offsets[i*3+j] = ui->tblOffsets->item(i,j)->
              text().toInt(&conv);
          ret &= conv;
        }
    }

  return ret;
}
