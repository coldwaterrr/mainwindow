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
    // ���ð�ť��СΪͼ���С
    circleBtn->setFixedSize(60, 60);
    // ����ͼ���С
    circleBtn->setIconSize(QSize(50,50));
    // ȥ���߿򡢱߾������
    // circleBtn->setFlat(true);  // ȥ������߿򣨿�ѡ��
    circleBtn->setText("");    // ȷ��û������
    QPushButton* squareBtn = new QPushButton("������");
    layout->addWidget(circleBtn);
    layout->addWidget(squareBtn);

    connect(circleBtn, &QPushButton::pressed, this, [=]() {
        startDrag("circle");
        });
    connect(squareBtn, &QPushButton::pressed, this, [=]() {
        startDrag("square");
        });

    // �����¼�����������׽��ť�Ľ�����뿪�¼�
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

    // ��ʱ���������Ϊͼ��
    QApplication::setOverrideCursor(QCursor(cursorPixmap, cursorPixmap.width() / 2, cursorPixmap.height() / 2));

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(cursorPixmap);
    drag->setHotSpot(QPoint(cursorPixmap.width() / 2, cursorPixmap.height() / 2));

    drag->exec(Qt::CopyAction);

    // �϶�������ָ����
    QApplication::restoreOverrideCursor();
}

// �¼����������ڲ�׽��ť����������뿪�¼�
bool ShapeLibrary::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::Enter) {
        if (QPushButton* button = qobject_cast<QPushButton*>(watched)) {
            setCursor(Qt::PointingHandCursor);  // ������ʱ���ı���Ϊ����
        }
    }
    else if (event->type() == QEvent::Leave) {
        if (QPushButton* button = qobject_cast<QPushButton*>(watched)) {
            setCursor(Qt::ArrowCursor);  // ����뿪ʱ���ָ�Ĭ�Ϲ��
        }
    }

    return QWidget::eventFilter(watched, event);  // �����¼�������
}