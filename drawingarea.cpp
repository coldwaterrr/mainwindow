#include "drawingarea.h"
#include <QPainter>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMenu>
#include <QAction>

DrawingArea::DrawingArea(QWidget* parent) : QWidget(parent)
{
    // 可选设置背景颜色
    setStyleSheet("background-color: white;");

    setAcceptDrops(true);
}

DrawingArea::~DrawingArea() 
{
    qDeleteAll(shapes);
}

Shape* DrawingArea::getSelectedShape() const
{
    return selectedShape; // 返回选中的图形
}

void DrawingArea::moveUp(Shape* shape) {
    int index = shapes.indexOf(shape);
    if (index != -1 && index < shapes.size() - 1) {
        // 上移一层
        shapes.move(index, index + 1);
        update();
    }
}

void DrawingArea::moveDown(Shape* shape) {
    int index = shapes.indexOf(shape);
    if (index > 0) {
        // 下移一层
        shapes.move(index, index - 1);
        update();
    }
}

void DrawingArea::moveToTop(Shape* shape) {
    int index = shapes.indexOf(shape);
    if (index != -1) {
        // 置顶
        shapes.move(index, shapes.size() - 1);
        update();
    }
}

void DrawingArea::moveToBottom(Shape* shape) {
    int index = shapes.indexOf(shape);
    if (index != -1) {
        // 置底
        shapes.move(index, 0);
        update();
    }
}

void DrawingArea::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 网格背景
    const int gridSize = 20;
    QPen pen(Qt::lightGray);
    pen.setWidth(1);
    painter.setPen(pen);

    // 画纵向线
    for (int x = 0; x < width(); x += gridSize) 
    {
        painter.drawLine(x, 0, x, height());
    }

    // 画横向线
    for (int y = 0; y < height(); y += gridSize) 
    {
        painter.drawLine(0, y, width(), y);
    }

    for (Shape* shape : shapes) {
        shape->draw(painter, shape == selectedShape);
    }
}

void DrawingArea::dragEnterEvent(QDragEnterEvent* event)
{
    event->acceptProposedAction();
}

void DrawingArea::dropEvent(QDropEvent* event)
{
    QString shapeType = event->mimeData()->text();
    QPoint pos = event->pos();

    Shape::ShapeType type = (shapeType == "circle") ? Shape::ShapeType::Circle : Shape::ShapeType::Square;
    shapes.append(new Shape(type, pos));
    update();
}

void DrawingArea::mousePressEvent(QMouseEvent* event) 
{
    if (event->button() == Qt::RightButton) 
    {
        selectedShape = nullptr;

        for (Shape* shape : shapes) 
        {
            if (shape->contains(event->pos())) 
            {
                selectedShape = shape;

                // 创建右键菜单
                QMenu contextMenu(this);

                QAction* copy = contextMenu.addAction("复制    Ctrl+C");
                QAction* cut = contextMenu.addAction("剪切    Ctrl+X");
                QAction* paste = contextMenu.addAction("粘贴    Ctrl+V");
                QAction* reuse = contextMenu.addAction("复用    Ctrl+D");
                QAction* deleteAction = contextMenu.addAction("删除  Delete/Backspace");

                // 添加图形层级操作
                QAction* moveUpAction = contextMenu.addAction("上移");
                QAction* moveDownAction = contextMenu.addAction("下移");
                QAction* moveToTopAction = contextMenu.addAction("置顶");
                QAction* moveToBottomAction = contextMenu.addAction("置底");

                connect(moveUpAction, &QAction::triggered, this, [=]() { moveUp(selectedShape); });
                connect(moveDownAction, &QAction::triggered, this, [=]() { moveDown(selectedShape); });
                connect(moveToTopAction, &QAction::triggered, this, [=]() { moveToTop(selectedShape); });
                connect(moveToBottomAction, &QAction::triggered, this, [=]() { moveToBottom(selectedShape); });


                QAction* selectedAction = contextMenu.exec(event->globalPos());

                update();
                return;
            }
        }
    }

    // 左键拖动逻辑保留
    if (event->button() == Qt::LeftButton) 
    {
        selectedShape = nullptr;
        for (int i = shapes.size() - 1; i >= 0; --i) {
            Shape* shape = shapes[i];
            if (shape->contains(event->pos())) 
            {
                selectedShape = shape;

                int index = 0;
                for (const QPoint& handle : shape->resizeHandles()) 
                {
                    QRect handleRect(handle.x() - 4, handle.y() - 4, 8, 8);
                    if (handleRect.contains(event->pos())) 
                    {
                        resizing = true;
                        resizingHandle = handle;
                        resizingHandleIndex = index;
                        return;
                    }
                    ++index;
                }

                dragging = true;
                dragOffset = event->pos() - shape->boundingRect().center();
                update();
                return;
            }
        }
        update();
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent* event) 
{
    // 是否悬停在某个控制点上
    bool overHandle = false;

    if (selectedShape) {
        for (const QPoint& handle : selectedShape->resizeHandles()) {
            QRect handleRect(handle.x() - 4, handle.y() - 4, 8, 8);
            if (handleRect.contains(event->pos())) {
                overHandle = true;

                // 可以根据方向设不同的光标类型：
                setCursor(Qt::SizeFDiagCursor);  // 统一使用一个缩放光标
                break;
            }
        }
    }

    if (!overHandle && !dragging && !resizing) {
        setCursor(Qt::ArrowCursor);  // 恢复默认光标
    }

    // 处理拖动
    if (dragging && selectedShape) {
        QPoint newPos = event->pos() - dragOffset;
        selectedShape->moveTo(newPos);
        update();
        return;
    }

    // 处理拉伸
    if (resizing && selectedShape) {
        QRect rect = selectedShape->boundingRect();
        QPoint center = rect.center();
        QSize newSize = QSize(abs(event->pos().x() - center.x()) * 2,
            abs(event->pos().y() - center.y()) * 2);
        selectedShape->setSize(newSize.width(), newSize.height());
        update();
        return;
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent* event) 
{
    Q_UNUSED(event);
    dragging = false;
}

void DrawingArea::keyPressEvent(QKeyEvent* event) 
{
    if (selectedShape && event->key() == Qt::Key_Delete) 
    {
        shapes.removeOne(selectedShape);
        delete selectedShape;
        selectedShape = nullptr;
        update();
    }
}

