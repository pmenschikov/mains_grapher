#include <QDebug>
#include "powers_widget.h"
#include "ui_powers_widget.h"

powers_widget::powers_widget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::powers_widget)
{
  ui->setupUi(this);
  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      ui->tableWidget->setItem(j,i,new QTableWidgetItem);
}

powers_widget::~powers_widget()
{
  delete ui;
}

void powers_widget::set_powers(CCS5451_controller *ctrl)
{
  for(int i=0; i<3; i++)
    {
      ui->tableWidget->item(i,0)->setText(QString("%1").arg(ctrl->p_power(i),0,'f',3));
      ui->tableWidget->item(i,1)->setText(QString("%1").arg(ctrl->s_power(i),0,'f',3));
      ui->tableWidget->item(i,2)->setText(QString("%1").arg(ctrl->pf(i),0,'f',3));
    }

}
