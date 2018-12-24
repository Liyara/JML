#ifndef JML_TRACE_H
#define JML_TRACE_H

#include "types.h"
#include <IO/IO.h>

#define JML_UNDEFINED_SLOPE 0xffae

namespace jml {

    void setEndpointIntersectionEnabled(bool);
    bool endpointIntersectionEnabled();

    template <typename D>
    class Trace {
    public:
        Trace(const Vertex &a, const Vertex &b) : vA(a), vB(b) {}
        template<typename O>
        Trace(const Trace<O> &t) : vA(t.vA), vB(t.vB) {}

        template <typename O>
        D &operator=(const Trace<O> &t) {vA = t.vA; vB = t.vB;}

        virtual bool hasPoint(const Vertex&) const = 0;
        virtual bool intersects(const D&) const = 0;

        long double slope() const {
            long double num = (abs(vB.y() - vA.y()) > JML_EPSILON? (vB.y() - vA.y()) : JML_EPSILON / 10.L);
            long double den = (abs(vB.x() - vA.x()) > JML_EPSILON? (vB.x() - vA.x()) : JML_EPSILON / 10.L);
            return num / den;
        }

        template <typename O>
        bool parallelTo(const Trace<O> &t) const {return abs(slope() - t.slope()) < JML_EPSILON;}

        const Vertex &startingPoint() const {return vA;}
        const Vertex &endingPoint() const {return vB;}
    protected:

        Vertex vA, vB;
    };
}

#endif // JML_TRACE_H
