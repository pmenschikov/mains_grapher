#ifndef CCALIBRATIONWIDGET_H
#define CCALIBRATIONWIDGET_H

#include <QDialog>

namespace Ui {
  class CCalibrationWidget;
}

class CCalibrationWidget : public QDialog
{
  Q_OBJECT

public:
  explicit CCalibrationWidget(QWidget *parent = nullptr);
  ~CCalibrationWidget();

private:
  Ui::CCalibrationWidget *ui;
};

#endif // CCALIBRATIONWIDGET_H
