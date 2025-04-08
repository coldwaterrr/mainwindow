#include "shape.h"

Shape::Shape(ShapeType type, const QPoint& pos)
    : type(type), pos(pos), width(40), height(40)
{}

void Shape::moveTo(const QPoint& newPos) 
{
    pos = newPos;
}

void Shape::setSize(int w, int h) 
{
    width = w;
    height = h;
}

bool Shape::contains(const QPoint& point) const 
{
    return boundingRect().contains(point);
}

QRect Shape::boundingRect() const 
{
    return QRect(pos.x() - width / 2, pos.y() - height / 2, width, height);
}


QList<QPoint> Shape::resizeHandles() const 
{
    QRect r = boundingRect();
    return 
    {
        r.topLeft(),
        r.topRight(),
        r.bottomLeft(),
        r.bottomRight()
    };
}

void Shape::draw(QPainter& painter, bool selected) const {
    QRect rect = boundingRect();
    if (selected) 
    {
        painter.setPen(Qt::red);
    }
    else 
    {
        painter.setPen(Qt::black);
    }

    // 设置填充颜色为白色
    painter.setBrush(Qt::white);  // ✅ 添加这一行

    if (type == Shape::ShapeType::Circle) 
    {
        painter.drawEllipse(rect);
    }
    else if (type == Shape::ShapeType::Square) 
    {
        painter.drawRect(rect);
    }

    // ✅ 绘制虚线外框和控制点
    if (selected) 
    {
        QPen dashPen(Qt::DashLine);
        dashPen.setColor(Qt::blue);
        painter.setPen(dashPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(rect);

        painter.setBrush(Qt::blue);
        int handleSize = 6;
        for (const QPoint& pt : resizeHandles()) 
        {
            painter.drawRect(QRect(pt.x() - handleSize / 2, pt.y() - handleSize / 2, handleSize, handleSize));
        }
    }
}

Shape::ShapeType Shape::getType() const 
{
    return type;
}
