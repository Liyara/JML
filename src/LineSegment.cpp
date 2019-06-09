#include "JML/Maths.h"
#include <JUtil/IO/IO.h>

namespace jml {
    LineSegment::LineSegment(const Vertex &a, const Vertex &b) : Trace(a, b) {}

    long double LineSegment::length() const {
        return distance(vA, vB);
    }

    bool LineSegment::intersects(const LineSegment &ls) const {
        if (ls.vA == vA || ls.vA == vB || ls.vB == vA || ls.vB == vB) return endpointIntersectionEnabled();

        Line la(vA, vB), lb(ls.vA, ls.vB);
        if (la.parallelTo(lb) && la.intersects(lb)) return (hasPoint(ls.vA) || hasPoint(ls.vB));

        char ccw1, ccw2;
        ccw1 = ccw(vA, vB, ls.vA) * ccw(vA, vB, ls.vB);
        ccw2 = ccw(ls.vA, ls.vB, vA) * ccw(ls.vA, ls.vB, vB);
        return (ccw1 <= 0) && (ccw2 <= 0);
    }

    bool LineSegment::hasPoint(const Vertex &p) const {
        if (!endpointIntersectionEnabled() && (p == vA || p == vB)) return false;
        if (ccw(vA, p, vB) == JML_COLLINEAR) {
            char
                cmpABX = compare(vA.x(), vB.x()),
                cmpAPX = compare(vA.x(), p.x()),
                cmpBPX = compare(vB.x(), p.x()),
                cmpAPY = compare(vA.y(), p.y()),
                cmpBPY = compare(vB.y(), p.y())
            ;
            if (cmpABX != JML_EQUAL) {
                return (
                    ((cmpAPX == JML_LESS || cmpAPX == JML_EQUAL) && (cmpBPX == JML_GREATER || cmpBPX == JML_EQUAL)) ||
                    ((cmpBPX == JML_LESS || cmpBPX == JML_EQUAL) && (cmpAPX == JML_GREATER || cmpAPX == JML_EQUAL))
                );
            }
            else {
                return (
                    ((cmpAPY == JML_LESS || cmpAPY == JML_EQUAL) && (cmpBPY == JML_GREATER || cmpBPY == JML_EQUAL)) ||
                    ((cmpBPY == JML_LESS || cmpBPY == JML_EQUAL) && (cmpAPY == JML_GREATER || cmpAPY == JML_EQUAL))
                );
            }
        } else return false;
    }

    Vertex LineSegment::midPoint() const {
        return (vA + vB) / 2.L;
    }

    bool LineSegment::intersects(const Line &l) const {
        return l.intersects(*this);
    }
}
