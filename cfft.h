#ifndef CFFT_H
#define CFFT_H

#include <fftw3.h>
#include <complex>

const int FFT_SIZE = 40;

template<class T>
struct Vector3P
{
  std::complex<T> a;
  std::complex<T> b;
  std::complex<T> c;
};

using Vec3P = Vector3P<double>;

class CFFT
{
public:
  CFFT();

  void prepare_plans();
  void execute();

  void debug_out();


  double magA(int freq);
  double angleA(int freq);
  double magB(int freq);
  double angleB(int freq);
  double magC(int freq);
  double angleC(int freq);

  Vec3P vector3(int freq);

public:
  double fft_inA[FFT_SIZE];
  double fft_inB[FFT_SIZE];
  double fft_inC[FFT_SIZE];

protected:
  fftw_plan fft_planA;
  fftw_plan fft_planB;
  fftw_plan fft_planC;

  fftw_complex fft_outA[FFT_SIZE];
  fftw_complex fft_outB[FFT_SIZE];
  fftw_complex fft_outC[FFT_SIZE];
};

#endif // CFFT_H
