#include "ccalibrationwidget.h"
#include "ui_ccalibrationwidget.h"

CCalibrationWidget::CCalibrationWidget(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CCalibrationWidget)
{
  ui->setupUi(this);
}

CCalibrationWidget::~CCalibrationWidget()
{
  delete ui;
}
