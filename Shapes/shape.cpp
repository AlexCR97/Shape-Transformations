#include "shape.h"

#include "Shapes/lineshape.h"

namespace cr {

    Shape::Shape() {}
    Shape::Shape(QList<Point> vertices)
    {
        this->setVertices(vertices);
    }

    void Shape::addPoint(Point p) {
        this->points.push_back(p);
    }

    void Shape::addPoints(QList<Point> points) {
        for (Point p : points)
            this->points.push_back(p);
    }

    void Shape::clearPoints() {
        this->points.clear();
    }

    QList<Point>& Shape::getPoints() {
        return this->points;
    }

    QList<Point>& Shape::getVertices()
    {
        return this->vertices;
    }

    void Shape::setVertices(QList<Point> vertices)
    {
        this->clearPoints();
        this->vertices = vertices;

        for (int i = 0; i < vertices.size(); i++) {
            int j = wrap(i + 1, 0, vertices.size() - 1);

            Point p1 = vertices[i];
            Point p2 = vertices[j];
            LineShape line(p1, p2);

            this->addPoints(line.getPoints());
        }
    }

    int Shape::wrap(int value, int min, int max)
    {
        if (value > max)
            return min;
        if (value < min)
            return max;
        return value;
    }

}

