#ifndef LINESHAPE_H
#define LINESHAPE_H

#include "point.h"
#include "shape.h"

namespace cr {

    class LineShape : public Shape
    {
    public:
        LineShape(Point p1, Point p2);
    };

}

#endif // LINESHAPE_H
