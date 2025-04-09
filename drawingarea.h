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

    // ��ȡѡ�е���״
    Shape* getSelectedShape() const;
    bool isCursorOverHandle(const QPoint& pos);
    void moveUp(Shape* shape);
    void moveDown(Shape* shape);
    void moveToTop(Shape* shape);
    void moveToBottom(Shape* shape);

protected:
    void paintEvent(QPaintEvent* event) override; // �ػ��¼�
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
    int resizingHandleIndex = -1; // ��ѡ

    QPoint dragOffset;  // ����������ͼ�����ĵ��ƫ��
    QPoint resizingHandle;  // ��ǰ����Ŀ��Ƶ�λ�ã���������
};

#endif // DRAWINGAREA_H
