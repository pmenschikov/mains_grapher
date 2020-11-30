#ifndef CCALIBRATIONWIDGET_H
#define CCALIBRATIONWIDGET_H

#include <QDialog>

#include <ccalibrationdata.h>

namespace Ui {
  class CCalibrationWidget;
}

class CCalibrationWidget : public QDialog
{
  Q_OBJECT

public:
  explicit CCalibrationWidget(QWidget *parent = nullptr);
  ~CCalibrationWidget();

  void set_calibration( const CCalibrationData& data);

  bool get_calibration(CCalibrationData&);

private:
  Ui::CCalibrationWidget *ui;
};

#endif // CCALIBRATIONWIDGET_H
