#ifndef JML_TRIG_H
#define JML_TRIG_H

#include "JML/misc.h"

namespace jml {

    class Angle;

    long double atan2(long double, long double);

    long double cos(long double);

    long double sin(long double);

    long double asin(long double);

    long double acos(long double);

    long double atan(long double);

    long double tan(long double);

    long double cot(long double);

    long double acot(long double);

    long double sec(long double);

    long double asec(long double);

    long double csc(long double);

    long double acsc(long double);

    long double sigmoid(long double);

    long double tanh(long double);


    long double cos(const Angle&);

    long double sin(const Angle&);

    long double asin(const Angle&);

    long double acos(const Angle&);

    long double atan(const Angle&);

    long double tan(const Angle&);

    long double cot(const Angle&);

    long double acot(const Angle&);

    long double sec(const Angle&);

    long double asec(const Angle&);

    long double csc(const Angle&);

    long double acsc(const Angle&);

    long double sigmoid(const Angle&);

    long double tanh(const Angle&);

}

#endif // JML_TRIG_H
