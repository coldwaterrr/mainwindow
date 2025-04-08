#include "shapelibrary.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include <QApplication>

ShapeLibrary::ShapeLibrary(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QPushButton* circleBtn = new QPushButton("圆形");
    QPushButton* squareBtn = new QPushButton("正方形");
    layout->addWidget(circleBtn);
    layout->addWidget(squareBtn);

    connect(circleBtn, &QPushButton::pressed, this, [=]() {
        startDrag("circle");
        });
    connect(squareBtn, &QPushButton::pressed, this, [=]() {
        startDrag("square");
        });
}

void ShapeLibrary::startDrag(const QString& shapeType) 
{
    QMimeData* mimeData = new QMimeData;
    mimeData->setText(shapeType);

    QPixmap cursorPixmap(32, 32);
    cursorPixmap.fill(Qt::transparent);
    QPainter p(&cursorPixmap);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(Qt::white);

    if (shapeType == "circle") {
        p.drawEllipse(0, 0, 30, 30);
    }
    else if (shapeType == "square") {
        p.drawRect(1, 1, 30, 30);
    }
    p.end();

    // 临时设置鼠标光标为图形
    QApplication::setOverrideCursor(QCursor(cursorPixmap, cursorPixmap.width() / 2, cursorPixmap.height() / 2));

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(cursorPixmap);
    drag->setHotSpot(QPoint(cursorPixmap.width() / 2, cursorPixmap.height() / 2));

    drag->exec(Qt::CopyAction);

    // 拖动结束后恢复鼠标
    QApplication::restoreOverrideCursor();
}