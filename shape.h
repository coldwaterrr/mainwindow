#ifndef SHAPE_H
#define SHAPE_H

#include <QPoint>
#include <QRect>
#include <QPainter>

class Shape 
{
public:
    enum class ShapeType { Circle, Square };

    Shape(ShapeType type, const QPoint& pos);
    virtual ~Shape() = default;

    void moveTo(const QPoint& newPos);
    void setSize(int w, int h);
    bool contains(const QPoint& point) const;

    void draw(QPainter& painter, bool selected = false) const;

    QRect boundingRect() const;
    ShapeType getType() const;
    QList<QPoint> resizeHandles() const;
private:
    ShapeType type;
    QPoint pos;
    int width;
    int height;
};

#endif // SHAPE_H
