#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
class ShapeLibrary : public QWidget
{
    Q_OBJECT
public:
    ShapeLibrary(QWidget* parent = nullptr);
    ~ShapeLibrary() {}

    void startDrag(const QString& shapeType);
    //void enterEvent(QEvent* event) override;
    //void leaveEvent(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
};