#include "vectordiagramwidget.h"
#include "ui_vectord.h"
#include <QStringList>
#include <QDebug>
#include <QPainter>
#include <QBrush>
#include <QPen>

#include "cfft.h"
#include <cmath>

QDebug operator <<(QDebug, const Vec3P&);
QDebug operator <<(QDebug, const std::complex<double>&);

VectorDiagramWidget::VectorDiagramWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::VectorD)
{
  ui->setupUi(this);
  for(int i=0; i<2; i++)
    for(int j=0; j<3; j++)
      ui->tableWidget->setItem(j,i,new QTableWidgetItem);

  QStringList vnames={"A", "B", "C"};
  ui->cbVec1->addItems(vnames);
  ui->cbVec2->addItems(vnames);
  ui->cbVec3->addItems(vnames);
  ab_angle = 0;
  ac_angle = 0;
}

VectorDiagramWidget::~VectorDiagramWidget()
{
  delete ui;
}

void VectorDiagramWidget::update_fft(CFFT* fft)
{
  ui->tableWidget->item(0,0)->setText(QString("%1").arg(fft->magA(1)));
  ui->tableWidget->item(0,1)->setText(QString("%1").arg(fft->angleA(1)));
  ui->tableWidget->item(1,0)->setText(QString("%1").arg(fft->magB(1)));
  ui->tableWidget->item(1,1)->setText(QString("%1").arg(fft->angleB(1)));
  ui->tableWidget->item(2,0)->setText(QString("%1").arg(fft->magC(1)));
  ui->tableWidget->item(2,1)->setText(QString("%1").arg(fft->angleC(1)));

  m_phases = fft->vector3(1);

  on_pushButton_clicked();
}

void VectorDiagramWidget::on_pushButton_clicked()
{
  ab_angle = std::arg(m_phases.a) - std::arg(m_phases.b);
  ac_angle = std::arg(m_phases.a) - std::arg(m_phases.c);

  m_a = std::abs(m_phases.a);
  m_b = std::abs(m_phases.b);
  m_c = std::abs(m_phases.c);

  double max_len = std::max({m_a, m_b, m_c});

  m_a /= max_len;
  m_b /= max_len;
  m_c /= max_len;

  repaint();

}

QDebug operator << (QDebug d, const Vec3P &v)
{
  d << std::abs(v.a) << std::abs(v.b) << std::abs(v.c);
  return d;
}

void VectorDiagramWidget::paintEvent(QPaintEvent*)
{
  QPainter painter(this);

  int centerX = 600;
  int centerY = 200;
  float len = 100.f;
  painter.drawEllipse(centerX-1, centerY-1, 3,3);

  QPen pen;
  pen.setWidth(2);
  pen.setColor(Qt::yellow);
  painter.setPen(pen);
  draw_vector(centerX, centerY, len*m_a, 0.0l, painter);


  pen.setColor(Qt::green);
  painter.setPen(pen);
  draw_vector(centerX, centerY, len*m_b, ab_angle, painter);

  pen.setColor(Qt::red);
  painter.setPen(pen);
  draw_vector(centerX, centerY, len*m_c, ac_angle, painter);


}

void VectorDiagramWidget::draw_vector(int cx, int cy, double len,
                          double angle,
                          QPainter& painter)
{
  int x = len*cos(angle-M_PI/2) + cx;
  int y = len*sin(angle-M_PI/2) + cy;
  painter.drawLine(cx, cy, x, y);
}
