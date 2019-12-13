#include "lineshape.h"

#include <QtGlobal>
#include <QtMath>

namespace cr {

    LineShape::LineShape(Point p1, Point p2)
    {
        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;

        double x = p1.x;
        double y = p1.y;

        double step = (qAbs(dx) > qAbs(dy))? qAbs(dx) : qAbs(dy);

        double xStep = dx / step;
        double yStep = dy / step;

        for (int i = 0; i <= step; i++) {
            this->addPoint(Point(x, y));
            x += xStep;
            y += yStep;
        }
    }

}
