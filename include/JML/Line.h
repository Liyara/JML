#ifndef JML_LINE_H
#define JML_LINE_H

#include <JML/Trace.h>

namespace jml {
    class Line : public Trace<Line> {
    public:
        Line(const Vertex &a, const Vertex &b) : Trace(a, b) {}
        Line(const Line &line) : Trace(line) {}

        bool hasPoint(const Vertex &p) const {
            Line bridge(vA, p);
            char cmp = compare(slope(), bridge.slope());
            return (cmp == JML_EQUAL);
        }

        bool intersects(const Line &line, Vertex *crossover) const {

            bool i = intersects(line);

            if (crossover && i) {
                long double cxd = slope() - line.slope();
                if (compare(cxd, 0) != JML_EQUAL) {
                    crossover->x() = (line.intercept().y() - intercept().y()) / cxd;
                    crossover->y() = (slope() * crossover->x()) + intercept().y();
                } else *crossover = line.vA;
            }

            return i;
        }

        bool intersects(const Line &line) const {
            if (parallelTo(line)) return (hasPoint(line.vB));
            else return true;
        }

        Vector2ld intercept() const {
            long double yIntercept = vA.y() - (slope() * vA.x());
            long double xIntercept = -yIntercept / slope();
            return {xIntercept, yIntercept};
        }
        /*LineSegment terminatingSegment() const {
            return LineSegment(vA, vB);
        }
        bool hasSegment(const LineSegment &seg) const {
            return hasPoint(seg.startingPoint()) && hasPoint(seg.endingPoint());
        }*/
    };
}

#endif // JML_LINE_H
