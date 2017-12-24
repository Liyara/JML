#ifndef JML_MISC_H
#define JML_MISC_H

#include "log.h"

namespace jml {

    jutil::Tuple<int64_t, long double> modf(long double);

    long double round(long double, uint8_t);

    long double round(long double);

    int16_t sign(long double);

    long double fma(long double, long double, long double);

    long double copysign(long double, long double);

    long double factorial(long unsigned);

    constexpr long double e();
}

#endif // JML_MISC_H
