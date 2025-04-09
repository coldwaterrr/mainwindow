#include "shapelibrary.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>

ShapeLibrary::ShapeLibrary(QWidget* parent) : QWidget(parent)
{
    // setMouseTracking(true);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QPushButton* circleBtn = new QPushButton(this);
    circleBtn->setIcon(QIcon("./icons/circle.png"));
    // 设置按钮大小为图标大小
    circleBtn->setFixedSize(60, 60);
    // 设置图标大小
    circleBtn->setIconSize(QSize(50,50));
    // 去掉边框、边距和文字
    // circleBtn->setFlat(true);  // 去除立体边框（可选）
    circleBtn->setText("");    // 确保没有文字
    QPushButton* squareBtn = new QPushButton("正方形");
    layout->addWidget(circleBtn);
    layout->addWidget(squareBtn);

    connect(circleBtn, &QPushButton::pressed, this, [=]() {
        startDrag("circle");
        });
    connect(squareBtn, &QPushButton::pressed, this, [=]() {
        startDrag("square");
        });

    // 设置事件过滤器来捕捉按钮的进入和离开事件
    circleBtn->installEventFilter(this);
    squareBtn->installEventFilter(this);
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

// 事件过滤器用于捕捉按钮的鼠标进入和离开事件
bool ShapeLibrary::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::Enter) {
        if (QPushButton* button = qobject_cast<QPushButton*>(watched)) {
            setCursor(Qt::PointingHandCursor);  // 鼠标进入时，改变光标为手型
        }
    }
    else if (event->type() == QEvent::Leave) {
        if (QPushButton* button = qobject_cast<QPushButton*>(watched)) {
            setCursor(Qt::ArrowCursor);  // 鼠标离开时，恢复默认光标
        }
    }

    return QWidget::eventFilter(watched, event);  // 传递事件给父类
}