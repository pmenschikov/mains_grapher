#include "cfft.h"
#include <complex>
#include <QDebug>

CFFT::CFFT()
{

}

void CFFT::prepare_plans()
{
  fft_planA = fftw_plan_dft_r2c_1d(40, fft_inA, fft_outA, FFTW_ESTIMATE);
  fft_planB = fftw_plan_dft_r2c_1d(40, fft_inB, fft_outB, FFTW_ESTIMATE);
  fft_planC = fftw_plan_dft_r2c_1d(40, fft_inC, fft_outC, FFTW_ESTIMATE);

}

void CFFT::execute()
{

  fftw_execute(fft_planA);
  fftw_execute(fft_planB);
  fftw_execute(fft_planC);
}

void CFFT::debug_out()
{

  for(int i=0; i<19; i+=1)
    {
      int idx=i;
  std::complex<double> v_a(fft_outA[idx][0], fft_outA[idx][1]);
  std::complex<double> v_b(fft_outB[idx][0], fft_outB[idx][1]);
  std::complex<double> v_c(fft_outC[idx][0], fft_outC[idx][1]);
  double diff = std::arg(v_a)*180.0/M_PI-std::arg(v_c)*180.0/M_PI;

      qDebug() << i << Qt::fixed << qSetRealNumberPrecision(4) << qSetFieldWidth(10)
                      << std::abs(v_a)*0.0732/FFT_SIZE
                      << std::abs(v_b)*0.0732/FFT_SIZE
                      << std::abs(v_c)*0.0732/FFT_SIZE
                      << std::arg(v_a)*180.0/M_PI
                      << std::arg(v_b)*180.0/M_PI
                      << std::arg(v_c)*180.0/M_PI
                      << diff;
    }
}

double CFFT::magA(int freq)
{
  std::complex<double> c(fft_outA[freq][0], fft_outA[freq][1]);

  return std::abs(c)/FFT_SIZE;
}

double CFFT::angleA(int freq)
{
  std::complex<double> c(fft_outA[freq][0], fft_outA[freq][1]);

  return std::arg(c)*180/M_PI;
}

double CFFT::magB(int freq)
{
  std::complex<double> c(fft_outB[freq][0], fft_outB[freq][1]);

  return std::abs(c)/FFT_SIZE;
}

double CFFT::angleB(int freq)
{
  std::complex<double> c(fft_outB[freq][0], fft_outB[freq][1]);

  return std::arg(c)*180/M_PI;
}

double CFFT::magC(int freq)
{
  std::complex<double> c(fft_outC[freq][0], fft_outC[freq][1]);

  return std::abs(c)/FFT_SIZE;
}

double CFFT::angleC(int freq)
{
  std::complex<double> c(fft_outC[freq][0], fft_outC[freq][1]);

  return std::arg(c)*180/M_PI;
}

Vec3P CFFT::vector3(int freq)
{
  std::complex<double> a(fft_outA[freq][0], fft_outA[freq][1]);
  std::complex<double> b(fft_outB[freq][0], fft_outB[freq][1]);
  std::complex<double> c(fft_outC[freq][0], fft_outC[freq][1]);
  double N = FFT_SIZE;

  return {a/N,b/N,c/N};
}
