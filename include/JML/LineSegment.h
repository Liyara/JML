#ifndef JML_LINE_SEGMENT_H
#define JML_LINE_SEGMENT_H

#include "JML/types.h"
#include "JML/Trace.h"

namespace jml {

    class Line;
    class Ray;

    class LineSegment : public Trace<LineSegment> {
    public:
        LineSegment(const Vertex&, const Vertex&);
        long double length() const;

        bool intersects(const LineSegment&) const override;
        bool intersects(const Line&) const;
        bool intersects(const Ray&) const;

        bool hasPoint(const Vertex&) const override;

        Vertex midPoint() const;
    };
}

#endif // JML_LINE_SEGMENT_H
