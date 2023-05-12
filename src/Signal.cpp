#define _USE_MATH_DEFINES
#include <cmath>
#include <complex>
#include <vector>
#include <algorithm>

#include "Signal.hpp"

template<typename T>
Signal<T>::Signal(const std::vector<T>& t, const std::vector<T>& X) : _X(X), _T(t)
{
    // Extract Signal Parameters Recording Frequency and Length
    _Fs = 1 / (_T(2) - _T(1));
    _L = _T.size();
    
    // Perform the fft
    fft();

    // Compute the two-sided spectrum P2. Then compute the single-sided spectrum P1 based on P2 and the even-valued signal length _L.
    _P2 = std::abs(_Y/_L);
    for (int i = 0; i < _L/2+1; i++)
    {
        _P1.push_back(_P2[i]);
    }

    for (int i = 1; i <= _P1.size()-1; i++)
    {
        _P1[i] = 2*_P1[i];
    }

    // Define the frequency domain f and plot the single-sided amplitude spectrum P1.
    for(int i =0; i<_L/2; i++)
        _f(i) = _Fs * i/_L;
    
    auto maxIt = std::max_element(_P1.begin(), _P1.end());

    double _maxFrequency = _f(maxIt - _P1.begin() );
    
}

template<typename T>
void Signal<T>::fft()
{
    //   For length N input vector x, the DFT is a length N vector X,
    //   with elements
    //                N
    //  X(k) =       sum  x(n)*exp(-j*2*pi*(k-1)*(n-1)/N), 1 <= k <= N.
    //               n=1
    const std::complex J(0, 1);
    const double N = _L;

    _Y.resize(N);
    for (std::size_t i = 0; i < N; ++i) {
       _Y[i] = static_cast<T>(0);
    }

    for (int k = 1; k <= N; k++)
    {
        for (int n = 1; n <= N; n++)
        {
            _Y(k-1) = _Y(k-1) + _X(n-1)*std::exp(-J*2*M_PI*(k-1)*(n-1)/N);
        }
    }
}