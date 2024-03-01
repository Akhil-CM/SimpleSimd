// -*- C++ -*-
#include "../simd.h"
#include <iostream>
#include <Vc/Vc>

using KFP::SIMD::SimdMask;
using KFP::SIMD::SimdIndex;
using KFP::SIMD::SimdF;
using KFP::SIMD::SimdI;

__KFP_SIMD__INLINE SimdF Sin(const SimdF& phi)
{
    const SimdF pi{3.1415926535897932f};
    const SimdF nTurnsF = (phi + pi) / (SimdF{2.f}*pi);
    SimdIndex nTurns = SimdIndex{nTurnsF};
    nTurns = select((nTurns <= 0) && (phi < -pi), nTurns-1, nTurns);

    const SimdF& x{ phi - SimdF(nTurns)*(SimdF(2.f)*pi) };

    const SimdF& B = SimdF{4.f}/pi;
    const SimdF& C = -B/pi;

    SimdF y = (B + C * abs(x)) * x;

    const SimdF& P{ 0.218f };
    y = P * (y * abs(y) - y) + y;

    return y;
}
__KFP_SIMD__INLINE SimdF Cos(const SimdF& phi)
{
    return Sin(phi + SimdF{1.570796326795f}); //x + pi/2
}

Vc::float_v SinVc( const Vc::float_v& phi )
{
    const Vc::float_v pi(3.1415926535897932f);
    const Vc::float_v nTurnsF = (phi + pi) / (Vc::float_v(2.f)*pi);
    Vc::int_v nTurns = simd_cast<Vc::int_v>( nTurnsF );
    nTurns( (nTurns<=Vc::int_v(Vc::Zero)) && simd_cast<Vc::int_m>(phi<-pi)) -= 1;

    const Vc::float_v& x = phi - simd_cast<Vc::float_v>(nTurns)*(Vc::float_v(2.f)*pi);

    const Vc::float_v& B = 4.f/pi;
    const Vc::float_v& C = -B/pi;

    Vc::float_v y = (B + C * Vc::abs(x)) * x;

    const Vc::float_v& P = 0.218f;
    y = P * (y * Vc::abs(y) - y) + y;

    return y;
}

template<typename T>
struct ApplySin
{
    T operator()(const T& val) const
    {
        return std::sin(val);
    }
};

int main()
{
    SimdMask simd_mask;
    SimdIndex simd_index;
    SimdF simd_float{ 0.0f };

    std::cout << "Print SimdF{0.0f}\n" ;
    std::cout << simd_float << "\n\n";
    std::cout << "Print SimdF{0.0f} + 5\n" ;
    std::cout << simd_float + 5 << "\n\n";
    std::cout << "Print SimdF{} + SimdF{5}\n" ;
    std::cout << (SimdF{} + SimdF{ 5 }) << "\n\n";

    SimdF::value_type f1234[SimdF::SimdLen]{1.0f, 2.0f, 3.0f, 4.0f};
    std::cout << "Print load_partial of {1.0f, 2.0f, 3.0f, 4.0f}\n" ;
    std::cout << SimdF{}.load_partial(5, f1234) << "\n\n";

    std::cout << "sin(1.0f), sin(2.0f), sin(3.0f), sin(4.0f):\n" ;
    std::cout << std::sin(1.0f) << ", " ;
    std::cout << std::sin(2.0f) << ", " ;
    std::cout << std::sin(3.0f) << ", " ;
    std::cout << std::sin(4.0f) << "\n" ;

    std::cout << "apply sin to Print load_partial of {1.0f, 2.0f, 3.0f, 4.0f}\n" ;
    std::cout << apply(SimdF{}.load_partial(5, f1234), ApplySin<float>{}) << "\n\n";

    std::cout << "Sin to Print load_partial of {1.0f, 2.0f, 3.0f, 4.0f}\n" ;
    std::cout << Sin(SimdF{}.load_partial(5, f1234)) << "\n\n";

    Vc::float_v float_vc;
    float_vc.load(f1234);
    std::cout << "Vc::float_v to Print load of {1.0f, 2.0f, 3.0f, 4.0f}\n" ;
    std::cout << float_vc << "\n\n";
    std::cout << "SinVc to Print load of {1.0f, 2.0f, 3.0f, 4.0f}\n" ;
    std::cout << SinVc(float_vc) << "\n\n";

}
