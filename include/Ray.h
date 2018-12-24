#ifndef JML_RAY_H
#define JML_RAY_H

#include "Line.h"

namespace jml {
    class Ray : public Trace<Ray> {
    public:
        Ray(const Vertex&, const Vertex&);

        bool intersects(const LineSegment&) const;
        bool intersects(const LineSegment&, Vertex*) const;

        bool intersects(const Line&) const;
        bool intersects(const Ray&) const override;

        bool hasPoint(const Vertex&) const override;

        Angle angle() const;

    private:
    };
}

#endif // JML_RAY_H
