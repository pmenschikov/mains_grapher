#ifndef VECTORDIAGRAMWIDGET_H
#define VECTORDIAGRAMWIDGET_H

#include <QWidget>
#include "ui_vectord.h"
#include <complex>
#include "cfft.h"


namespace Ui {
  class VectorD;
}

class VectorDiagramWidget: public QWidget
{
  Q_OBJECT

public:
  explicit VectorDiagramWidget(QWidget *parent = nullptr);
  ~VectorDiagramWidget();

protected:
  void paintEvent(QPaintEvent*);

public slots:
  void update_fft(CFFT*);

private slots:
  void on_pushButton_clicked();

private:
  void draw_vector(int,int, double, double, QPainter&);
private:
  Ui::VectorD *ui;

  Vec3P m_phases;
  Vec3P m_vectors; // for draw
  double ab_angle, ac_angle;
  double m_a, m_b, m_c;
};

#endif // VECTORDIAGRAMWIDGET_H
