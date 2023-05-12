#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <complex>
#include <vector>
#include <algorithm>

#include "Signal.hpp"

Signal::Signal(const std::vector<double>& t, const std::vector<double>& X) : _X(X), _T(t)
{
    // Extract Signal Parameters Recording Frequency and Length
    _Fs = 1 / (_T[2] - _T[1]);
    _L = static_cast<double>( _T.size() );
    
#ifdef FALSE
    std::cout << "Fs: " << _Fs << std::endl << "L: " << _L << std::endl;
#endif

    // Perform the fft
    fft();

#ifdef FALSE
    for (int i = 0; i < _Y.size(); i++ ) 
        std::cout << "Y[ " << i << "]: " << _Y[i] << std::endl;
#endif

    // Compute the two-sided spectrum P2. Then compute the single-sided spectrum P1 based on P2 and the even-valued signal length _L.
    for (int i = 0; i < _Y.size(); i++)
    {
        _P2.push_back(std::abs(_Y[i] / std::complex(_L, 0.0)));
    }

#ifdef FALSE
    for (int i = 0; i < _P2.size(); i++ ) 
        std::cout << "P2[ " << i << "]: " << _P2[i] << std::endl;
#endif

    std::vector<double> temp;
    for (int i = 0; i < _L / 2 + 1; i++)
    {
        temp.push_back(_P2[i]);
    }
    for (int i = 1; i <= temp.size() - 1; i++)
    {
        _P1.push_back(2 * temp[i]);
    }

#ifdef FALSE
    for (int i = 0; i < _P1.size(); i++ ) 
        std::cout << "P1[ " << i << "]: " << _P1[i] << std::endl;
#endif

    // Define the frequency domain f and plot the single-sided amplitude spectrum P1.
    for(int i = 0; i < _L / 2; i++)
        _f.push_back(_Fs * static_cast<double>(i) / _L);

#ifdef FALSE
    for (int i = 0; i < _f.size(); i++ ) 
        std::cout << "f[ " << i << "]: " << _f[i] << std::endl;
#endif

    auto maxIt = std::max_element(_P1.begin(), _P1.end());
    int idx = maxIt - _P1.begin();

#ifdef FALSE
    std::cout << "f[0]:" << _f[0] <<std::endl;
    for (int i = idx-10; i < idx+10; i++ ) 
        std::cout << "P1[" << i << "]: " << _P1[i]
                    << "\tf[" << i << "]: " << _f[i] << std::endl;
#endif

    _maxFrequency = _f[idx];

#ifdef FALSE
        std::cout << "Max: " << _maxFrequency << " @ index: " << idx << std::endl;
#endif
}

void Signal::fft()
{
    //   For length N input vector x, the DFT is a length N vector X,
    //   with elements
    //                N
    //  X(k) =       sum  x(n)*exp(-j*2*pi*(k-1)*(n-1)/N), 1 <= k <= N.
    //               n=1
    const std::complex J(0.0, 1.0);
    const double N = _L;

    _Y.resize(N);
    for (std::size_t i = 0; i < N; ++i) {
       _Y[i] = static_cast<double>(0);
    }

    for (int k = 1; k <= N; k++)
    {
        for (int n = 1; n <= N; n++)
        {
            _Y[k-1] = _Y[k-1] + _X[n-1] * std::exp(-J * 2.0 * M_PI * (k - 1.0) * (n - 1.0) / N);
        }
    }
}