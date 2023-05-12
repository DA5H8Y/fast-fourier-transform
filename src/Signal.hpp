#include <vector>
#include <complex>

class Signal
{
 public:
   Signal(const std::vector<double>& t, const std::vector<double>& X);
   double getMaxFrequency() const { return _maxFrequency; }

 private:
   std::vector<double> _T;
   std::vector<double> _X;
   std::vector<std::complex<double>> _Y;
   std::vector<double> _P2;
   std::vector<double> _P1;    
   std::vector<double> _f;

   double _Fs;
   double _maxFrequency;
   double _L;

   /* FFT(X) is the discrete Fourier transform (DFT) of vector X.
      For length N input vector x, the DFT is a length N vector X,
      with elements
                    N
      X(k) =       sum  x(n)*exp(-j*2*pi*(k-1)*(n-1)/N), 1 <= k <= N.
                   n=1
   */
   void fft();   
};
