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
    QPoint topLeft = r.topLeft();
    QPoint topRight = r.topRight();
    QPoint bottomLeft = r.bottomLeft();
    QPoint bottomRight = r.bottomRight();

    QPoint topMid((topLeft.x() + topRight.x()) / 2, topLeft.y());
    QPoint bottomMid((bottomLeft.x() + bottomRight.x()) / 2, bottomLeft.y());
    QPoint leftMid(topLeft.x(), (topLeft.y() + bottomLeft.y()) / 2);
    QPoint rightMid(topRight.x(), (topRight.y() + bottomRight.y()) / 2);

    return {
        topLeft,
        topRight,
        bottomLeft,
        bottomRight,
        topMid,
        bottomMid,
        leftMid,
        rightMid
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
        int buttonSize = 10;
        QList<QPoint> handles = resizeHandles();
        for (const QPoint& pt : handles) 
        {
            painter.drawRect(QRect(pt.x() - handleSize / 2, pt.y() - handleSize / 2, handleSize, handleSize));
        }
        
        // ✅ 四个边中点的附加圆形按钮
        painter.setBrush(Qt::darkGreen);  // 你可以更改颜色
        int circleOffset = 15; // 与控制点的间距（单位：像素）

        // 对应 resizeHandles() 中的顺序：0~3 是四角，4~7 是四边中点
        // 四个方向的偏移（向外偏移一段距离）
        QVector<QPoint> offsets = {
            QPoint(0, -circleOffset),  // 上中：向上偏
            QPoint(0, circleOffset),   // 下中：向下偏
            QPoint(-circleOffset, 0),  // 左中：向左偏
            QPoint(circleOffset, 0)    // 右中：向右偏
        };

        for (int i = 0; i < 4; ++i) {  // 只处理中点（索引 4~7）
            QPoint midPoint = handles[4 + i];
            QPoint circleCenter = midPoint + offsets[i];
            painter.drawEllipse(QRect(circleCenter.x() - buttonSize / 2, circleCenter.y() - buttonSize / 2, buttonSize, buttonSize));
        }
    }
}

Shape::ShapeType Shape::getType() const 
{
    return type;
}
