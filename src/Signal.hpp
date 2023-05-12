#include <vector>
#include <complex>

template <typename T> 
class Signal
{
 public:
   Signal(const std::vector<T>& t, const std::vector<T>& X);
   T getMaxFrequency() const { return _maxFrequency; }
 private:
   std::vector<T> _T;
   std::vector<T> _X;
   std::vector<std::complex<T>> _Y;
   std::vector<T> _P2;
   std::vector<T> _P1;    
   std::vector<T> _f;

   T _Fs;
   T _maxFrequency;
   int _L;

   /* FFT(X) is the discrete Fourier transform (DFT) of vector X.
      For length N input vector x, the DFT is a length N vector X,
      with elements
                    N
      X(k) =       sum  x(n)*exp(-j*2*pi*(k-1)*(n-1)/N), 1 <= k <= N.
                   n=1
   */
   void fft();   
};
