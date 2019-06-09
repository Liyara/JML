#ifndef JML_LOG_H
#define JML_LOG_H

#include <JUtil/Core/Tuple.hpp>
#include <JUtil/Core/size_t.h>
#include <JUtil/Core/integers.h>

namespace jml {

    long double abs(long double);

    long double _ipow(long double, long double, long, long);

    long double log(long double, long double);

    long double log(long double);

    long double exp(long double);


    template <typename T>
    long double pow(long double n, T z) {
        return _ipow(n, n, static_cast<long>(z), 1);
    }

    template <>
    long double pow<long double>(long double, long double);

    template <>
    long double pow<double>(long double, double);

    template <>
    long double pow<float>(long double, float);

    long double ln(long double);

    long double root(long double, long double);

    long double sqrt(long double);

    float sqrtf(float); //FAST (less precise) sqrt algorithm

}

#endif // JML_LOG_H
