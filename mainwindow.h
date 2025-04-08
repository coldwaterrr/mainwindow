#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QMainWindow>
#include <QStackedWidget>
#include "ui_mainwindow.h"

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

private:
    Ui::mainwindowClass ui;
    QWidget* createToolBar(const QStringList& actions);
    QStackedWidget* toolBarContainer = nullptr;
};
