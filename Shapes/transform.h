#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Shapes/point.h"
#include "Shapes/shape.h"

namespace cr {

    class Transform
    {
    public:
        static void translate(cr::Shape& s, cr::Point factors);
        static void scale(cr::Shape& s, cr::Point factors);
        static void rotate(cr::Shape& s, int degrees);
        static void rotate(cr::Shape& s, int degrees, cr::Point origin);

    private:
        double **multiplyMatrices(double **matrix1, int rows1, int cols1, double **matrix2, int rows2, int cols2);
    };

}

#endif // TRANSFORM_H
