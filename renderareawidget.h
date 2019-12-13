#ifndef RENDERAREAWIDGET_H
#define RENDERAREAWIDGET_H

#include <QWidget>

#include "Shapes/point.h"
#include "Shapes/shape.h"
#include "Shapes/shapemodel.h"

class RenderAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RenderAreaWidget(QWidget *parent = nullptr);

private:
    QColor colorAxis;
    QColor colorBackground;
    QColor colorGrid;

    int pixelSize = 5;
    double scale = 5;

    QHash<QString, cr::ShapeModel> shapes;

public:
    void setScale(double scale);

private:
    void drawBackground(QPaintEvent *e);
    void drawGrid(QPaintEvent *e);
    void drawAxis(QPaintEvent *e);
    void drawShape(QPaintEvent *e, cr::ShapeModel);
    void drawShapes(QPaintEvent *e);

    cr::Point getOrigin();

public:
    void addShape(cr::ShapeModel s);
    void clearShapes();
    bool checkShape(QString id);
    cr::ShapeModel& getShape(QString id);
    void removeShape(QString id);

public:
    void applyTransformTranslate(QString id, cr::Point factors);
    void applyTransformScale(QString id, cr::Point factors);
    void applyTransformRotate(QString id, int degrees);

protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;

signals:

public slots:


};

#endif // RENDERAREAWIDGET_H
