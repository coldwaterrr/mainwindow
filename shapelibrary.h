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
};