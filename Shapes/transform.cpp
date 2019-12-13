#include "transform.h"

#include <QtMath>

namespace cr {

    void Transform::translate(cr::Shape& s, cr::Point factors)
    {
        for (Point& p : s.getVertices()) {
            p.x += factors.x;
            p.y += factors.y;
        }

        s.setVertices(s.getVertices());
    }

    void Transform::scale(cr::Shape& s, cr::Point factors)
    {
        for (Point& p : s.getVertices()) {
            p.x *= factors.x;
            p.y *= factors.y;
        }

        s.setVertices(s.getVertices());
    }

    void Transform::rotate(cr::Shape& s, int degrees)
    {
        double radians = qDegreesToRadians((double) degrees);
        double cos = qCos(radians);
        double sin = qSin(radians);

        for (Point& p : s.getVertices()) {
            double x = (p.x * cos) - (p.y * sin);
            double y = (p.x * sin) + (p.y * cos);

            p.x = x;
            p.y = y;
        }

        s.setVertices(s.getVertices());
    }

    void Transform::rotate(cr::Shape& s, int degrees, cr::Point origin)
    {
        double radians = qDegreesToRadians((double) degrees);
        double cos = qCos(radians);
        double sin = qSin(radians);

        for (Point& p : s.getVertices()) {
            p.x -= origin.x;
            p.y -= origin.y;

            double x = (p.x * cos) - (p.y * sin);
            double y = (p.x * sin) + (p.y * cos);

            p.x = x + origin.x;
            p.y = y + origin.y;
        }

        s.setVertices(s.getVertices());
    }

    double **Transform::multiplyMatrices(double **matrix1, int rows1, int cols1, double **matrix2, int rows2, int cols2)
    {
        if (cols1 != rows1)
            return nullptr;

        double **matrix = new double*[rows1];

        for (int i = 0; i < rows1; i++) {
            matrix[i] = new double[cols2];

            for (int j = 0; j < cols2; j++)
                for (int k = 0; k < cols1; k++)
                    matrix[i][j] += matrix1[i][k] * matrix2[k][j];
        }

        return matrix;
    }

}





































