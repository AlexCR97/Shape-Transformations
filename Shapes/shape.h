#ifndef SHAPE_H
#define SHAPE_H

#include <QList>

#include "point.h"

namespace cr {

    class Shape
    {
    public:
        Shape();
        Shape(QList<Point> vertices);

    protected:
        QList<Point> points;
        QList<Point> vertices;

    public:
        void addPoint(Point p);
        void addPoints(QList<Point> points);
        void clearPoints();
        QList<Point>& getPoints();

        QList<Point>& getVertices();
        void setVertices(QList<Point> vertices);

    private:
        int wrap(int value, int min, int max);
    };

}

#endif // SHAPE_H
