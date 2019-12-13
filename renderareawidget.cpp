#include "renderareawidget.h"

#include <QDebug>
#include <QPaintEvent>
#include <QPainter>

#include "Shapes/transform.h"

RenderAreaWidget::RenderAreaWidget(QWidget *parent) :
    QWidget(parent),
    colorAxis(80, 80, 80),
    colorBackground(250, 250, 250),
    colorGrid(110, 110, 110)
{

}

void RenderAreaWidget::setScale(double scale)
{
    this->scale = scale;
    this->repaint();
}

// DRAW /////////////////////////////////////////////////////////////////////////////////////////////////////

void RenderAreaWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    drawBackground(e);
    drawAxis(e);
    drawGrid(e);
    drawShapes(e);
}

void RenderAreaWidget::drawBackground(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setBrush(this->colorBackground);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.drawRect(this->rect());
}

void RenderAreaWidget::drawGrid(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(colorGrid);

    int areaWidth = this->rect().width();
    int areaHeight = this->rect().height();

    for (int i = 0; i < areaWidth; i++) {
        int x1 = (int) (pixelSize * i * scale);
        int y1 = 0;
        int x2 = x1;
        int y2 = (int) (areaHeight);
        p.drawLine(x1, y1, x2, y2);
    }

    for (int i = 0; i < areaHeight; i++) {
        int x1 = 0;
        int y1 = (int) (pixelSize * i * scale);
        int x2 = (int) areaWidth;
        int y2 = y1;
        p.drawLine(x1, y1, x2, y2);
    }
}

void RenderAreaWidget::drawAxis(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(colorAxis);

    int width = this->rect().width();
    int height = this->rect().height();
    cr::Point origin = this->getOrigin();

    int x = 0;
    int y = (int) (origin.y * pixelSize * scale);
    int w = (int) (width);
    int h = (int) (pixelSize * scale);

    p.fillRect(x, y, w, h, colorAxis);

    x = (int) (origin.x * pixelSize * scale);
    y = 0;
    w = (int) (pixelSize * scale);
    h = (int) height;

    p.fillRect(x, y, w, h, colorAxis);
}

void RenderAreaWidget::drawShape(QPaintEvent *e, cr::ShapeModel s)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(s.color);

    cr::Point origin = this->getOrigin();

    for (cr::Point point : s.shape.getPoints()) {
        int w = (int) (pixelSize * scale);
        int h = (int) (pixelSize * scale);

        int x = (int) (qRound(point.x) * w);
        int y = (int) (qRound(point.y) * h);

        int xOffset = (int) (origin.x * w);
        int yOffset = (int) (origin.y * h);

        p.fillRect(x + xOffset, -y + yOffset, w, h, s.color);
    }
}

void RenderAreaWidget::drawShapes(QPaintEvent *e)
{
    QHash<QString, cr::ShapeModel>::Iterator i;
    for (i = this->shapes.begin(); i != this->shapes.end(); i++)
        this->drawShape(e, i.value());
}

cr::Point RenderAreaWidget::getOrigin()
{
    int width = this->rect().width();
    int height = this->rect().height();

    double x = (width  / (pixelSize * scale)) / 2;
    double y = (height / (pixelSize * scale)) / 2;

    return cr::Point((int) x, (int) y);
}

// SHAPES /////////////////////////////////////////////////////////////////////////////////////////////////////

void RenderAreaWidget::addShape(cr::ShapeModel s)
{
    QString id = s.id;
    this->shapes[id] = s;
    this->repaint();
}

bool RenderAreaWidget::checkShape(QString id)
{
    if (this->shapes.contains(id))
        return true;
    return false;
}

void RenderAreaWidget::clearShapes()
{
    this->shapes.clear();
    this->repaint();
}

cr::ShapeModel& RenderAreaWidget::getShape(QString id)
{
    return this->shapes[id];
}

void RenderAreaWidget::removeShape(QString id)
{
    this->shapes.remove(id);
    this->repaint();
}

// TRANSFORMATIONS ////////////////////////////////////////////////////////////////////////////////////////////

void RenderAreaWidget::applyTransformTranslate(QString id, cr::Point factors)
{
    cr::ShapeModel& model = this->getShape(id);
    cr::Shape& s = model.getShape();

    cr::Transform::translate(s, factors);

    this->repaint();
}

void RenderAreaWidget::applyTransformScale(QString id, cr::Point factors)
{
    cr::ShapeModel& model = this->getShape(id);
    cr::Shape& s = model.getShape();

    cr::Transform::scale(s, factors);

    this->repaint();
}

void RenderAreaWidget::applyTransformRotate(QString id, int degrees)
{
    cr::ShapeModel& model = this->getShape(id);
    cr::Shape& s = model.getShape();

    cr::Transform::rotate(s, degrees);

    this->repaint();
}




















