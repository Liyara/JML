#ifndef JML_LINE_H
#define JML_LINE_H

#include "LineSegment.h"

namespace jml {
    class Line : public Trace<Line> {
    public:
        Line(const Vertex&, const Vertex&);
        Line(const Line&);
        bool hasPoint(const Vertex&) const override;
        bool intersects(const Line&) const override;
        bool intersects(const Line&, Vertex*) const;

        template <typename O, typename = typename jutil::Enable<!(jutil::IsSame<O, Line>::Value)>::Type>
        bool intersects(const Trace<O> &t) const {
            Line line(t.startingPoint(), t.endingPoint());
            Vertex ip;
            if (intersects(line, &ip)) return t.hasPoint(ip);
            else return false;
        }

        Vector2ld intercept() const;
        LineSegment terminatingSegment() const;
        bool hasSegment(const LineSegment&) const;
    };
}

#endif // JML_LINE_H
