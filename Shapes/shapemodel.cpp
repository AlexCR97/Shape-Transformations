#include "shapemodel.h"

namespace cr {

    ShapeModel::ShapeModel() {}

    ShapeModel::ShapeModel(QString id, Shape shape, QColor color)
    {
        this->id = id;
        this->shape = shape;
        this->color = color;
    }

    Shape& ShapeModel::getShape()
    {
        return this->shape;
    }

}
