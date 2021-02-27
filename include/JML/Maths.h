#ifndef JML_H
#define JML_H

#ifndef __cplusplus
    #error This library requires the ISO 2011 C++ standard.
#else
    #if __cplusplus < 201103L
        #error This library requires the ISO 2011 C++ standard.
    #endif
#endif

#include <JML/Ray.h>
#include <JML/Matrix.h>
#include <JML/Fraction.hpp>

#endif // JML_H
