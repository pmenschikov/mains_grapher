#include "ccalibrationwidget.h"
#include "ui_ccalibrationwidget.h"

CCalibrationWidget::CCalibrationWidget(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CCalibrationWidget)
{
  ui->setupUi(this);
  for(int i=0; i<2; i++)
    for(int j=0; j<3; j++)
      {
        ui->tableWidget->setItem(i,j,new QTableWidgetItem);
      }
}

CCalibrationWidget::~CCalibrationWidget()
{
  delete ui;
}

void CCalibrationWidget::set_calibration(const CCalibrationData &data)
{
  ui->tableWidget->item(0,0)->setText(QString("%1").arg(data.voltage_calib(0),0,'f',7));
  ui->tableWidget->item(0,1)->setText(QString("%1").arg(data.voltage_calib(2),0,'f',7));
  ui->tableWidget->item(0,2)->setText(QString("%1").arg(data.voltage_calib(4),0,'f',7));

  ui->tableWidget->item(1,0)->setText(QString("%1").arg(data.current_calib(1,false),0,'f',7));
  ui->tableWidget->item(1,1)->setText(QString("%1").arg(data.current_calib(3,false),0,'f',7));
  ui->tableWidget->item(1,2)->setText(QString("%1").arg(data.current_calib(5,false),0,'f',7));
}

bool CCalibrationWidget::get_calibration(CCalibrationData &data)
{
  bool ret = true;

  auto tw = ui->tableWidget;
  bool conv;

  data.m_data[0]=tw->item(0,0)->text().toFloat(&conv);
  ret += conv;
  data.m_data[2]=tw->item(0,1)->text().toFloat(&conv);
  ret += conv;
  data.m_data[4]=tw->item(0,2)->text().toFloat(&conv);
  ret += conv;

  data.m_data[1]=tw->item(1,0)->text().toFloat(&conv);
  ret += conv;
  data.m_data[3]=tw->item(1,1)->text().toFloat(&conv);
  ret += conv;
  data.m_data[5]=tw->item(1,2)->text().toFloat(&conv);
  ret += conv;

  return conv;
}
