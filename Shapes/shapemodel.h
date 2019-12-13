#ifndef SHAPEMODEL_H
#define SHAPEMODEL_H

#include <QColor>

#include "Shapes/shape.h"

namespace cr {

    class ShapeModel
    {
    public:
        ShapeModel();
        ShapeModel(QString id, Shape shape, QColor color);

    public:
        QString id;
        Shape shape;
        QColor color;

    public:
        Shape& getShape();
    };

}

#endif // SHAPEMODEL_H
