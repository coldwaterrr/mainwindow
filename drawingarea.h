#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H
#pragma execution_character_set("utf-8")
#include "shape.h"
#include <QWidget>

class DrawingArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingArea(QWidget* parent = nullptr);
    ~DrawingArea();

    // 获取选中的形状
    Shape* getSelectedShape() const;
    bool isCursorOverHandle(const QPoint& pos);
    void moveUp(Shape* shape);
    void moveDown(Shape* shape);
    void moveToTop(Shape* shape);
    void moveToBottom(Shape* shape);

protected:
    void paintEvent(QPaintEvent* event) override; // 重绘事件
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QVector<Shape*> shapes;
    Shape* selectedShape = nullptr;
    bool dragging = false;
    bool resizing = false;
    int resizingHandleIndex = -1; // 可选

    QPoint dragOffset;  // 鼠标点击点相对图形中心点的偏移
    QPoint resizingHandle;  // 当前点击的控制点位置，用于拉伸
};

#endif // DRAWINGAREA_H
