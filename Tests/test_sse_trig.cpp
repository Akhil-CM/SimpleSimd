// -*- C++ -*-
#include "../simd.h"
#include <iostream>
#include <Vc/Vc>

using KFP::SIMD::simd_mask;
using KFP::SIMD::simd_index;
using KFP::SIMD::simd_float;

__KFP_SIMD__INLINE simd_float Sin(const simd_float& phi)
{
    const simd_float pi{3.1415926535897932f};
    const simd_float nTurnsF = (phi + pi) / (simd_float{2.f}*pi);
    simd_index nTurns = simd_index{nTurnsF};
    nTurns = select((nTurns <= 0) && (phi < -pi), nTurns-1, nTurns);

    const simd_float& x{ phi - simd_float(nTurns)*(simd_float(2.f)*pi) };

    const simd_float& B = simd_float{4.f}/pi;
    const simd_float& C = -B/pi;

    simd_float y = (B + C * abs(x)) * x;

    const simd_float& P{ 0.218f };
    y = P * (y * abs(y) - y) + y;

    return y;
}
__KFP_SIMD__INLINE simd_float Cos(const simd_float& phi)
{
    return Sin(phi + simd_float{1.570796326795f}); //x + pi/2
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
    simd_mask mask;
    simd_index index;
    simd_float simd_f{ 0.0f };

    std::cout << "Print simd_float{0.0f}\n" ;
    std::cout << simd_f << "\n\n";
    std::cout << "Print simd_float{0.0f} + 5\n" ;
    std::cout << simd_f + 5 << "\n\n";
    std::cout << "Print simd_float{} + simd_float{5}\n" ;
    std::cout << (simd_float{} + simd_float{ 5 }) << "\n\n";

    simd_float::value_type f1234[simd_float::SimdLen]{1.0f, 2.0f, 3.0f, 4.0f};
    std::cout << "Print load_partial of {1.0f, 2.0f, 3.0f, 4.0f}\n" ;
    std::cout << simd_float{}.load_partial(5, f1234) << "\n\n";

    std::cout << "sin(1.0f), sin(2.0f), sin(3.0f), sin(4.0f):\n" ;
    std::cout << std::sin(1.0f) << ", " ;
    std::cout << std::sin(2.0f) << ", " ;
    std::cout << std::sin(3.0f) << ", " ;
    std::cout << std::sin(4.0f) << "\n" ;

    std::cout << "apply sin to Print load_partial of {1.0f, 2.0f, 3.0f, 4.0f}\n" ;
    std::cout << apply(simd_float{}.load_partial(5, f1234), ApplySin<float>{}) << "\n\n";

    std::cout << "Sin to Print load_partial of {1.0f, 2.0f, 3.0f, 4.0f}\n" ;
    std::cout << Sin(simd_float{}.load_partial(5, f1234)) << "\n\n";

    Vc::float_v float_vc;
    float_vc.load(f1234);
    std::cout << "Vc::float_v to Print load of {1.0f, 2.0f, 3.0f, 4.0f}\n" ;
    std::cout << float_vc << "\n\n";
    std::cout << "SinVc to Print load of {1.0f, 2.0f, 3.0f, 4.0f}\n" ;
    std::cout << SinVc(float_vc) << "\n\n";

}
