#ifndef POWERS_H
#define POWERS_H

#include <QWidget>
#include "ccs5451_controller.h"

namespace Ui {
  class powers_widget;
}

class powers_widget : public QWidget
{
  Q_OBJECT

public:
  explicit powers_widget(QWidget *parent = nullptr);
  ~powers_widget();

public slots:
  void set_powers(CCS5451_controller *ctrl);
private:
  Ui::powers_widget *ui;
};

#endif // POWERS_H
