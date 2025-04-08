#include "mainwindow.h"
#include "drawingarea.h"
#include "shapelibrary.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QWidget>
#include <QStackedWidget>

mainwindow::mainwindow(QWidget* parent)
    : QMainWindow(parent)
{
    // ---------- 顶部整体容器 ----------
    QWidget* topWidget = new QWidget(this);
    QVBoxLayout* topLayout = new QVBoxLayout(topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(0);

    // ---------- 菜单栏 ----------
    QWidget* menuBarWidget = new QWidget(this);
    QHBoxLayout* menuLayout = new QHBoxLayout(menuBarWidget);
    menuLayout->setContentsMargins(0, 0, 0, 0);
    menuLayout->setSpacing(50);

    QPushButton* btnStart = new QPushButton("开始");
    QPushButton* btnArrange = new QPushButton("排列");
    QPushButton* btnPage = new QPushButton("页面");

    QFont font;
    font.setPointSize(12);
    btnStart->setFont(font);
    btnArrange->setFont(font);
    btnPage->setFont(font);

    QString menuBtnStyle = "QPushButton { border: none; background: transparent; padding: 8px; }"
        "QPushButton:hover { color: blue; }";
    btnStart->setStyleSheet(menuBtnStyle);
    btnArrange->setStyleSheet(menuBtnStyle);
    btnPage->setStyleSheet(menuBtnStyle);

    menuLayout->addStretch();
    menuLayout->addWidget(btnStart);
    menuLayout->addWidget(btnArrange);
    menuLayout->addWidget(btnPage);
    menuLayout->addStretch();

    topLayout->addWidget(menuBarWidget);

    // ---------- 工具栏容器 ----------
    toolBarContainer = new QStackedWidget(this);
    toolBarContainer->setFixedHeight(40); // 设置工具栏高度
    topLayout->addWidget(toolBarContainer);

    setMenuWidget(topWidget); // 替代 setMenuBar()

    // ---------- 工具栏内容 ----------
    QWidget* startBar = createToolBar({ "填充样式", "粗体", "斜体", "下划线" });
    QWidget* arrangeBar = createToolBar({ "上移", "下移", "置顶", "置底" });
    QWidget* pageBar = createToolBar({ "背景颜色", "页面大小" });

    toolBarContainer->addWidget(startBar);
    toolBarContainer->addWidget(arrangeBar);
    toolBarContainer->addWidget(pageBar);

    toolBarContainer->setCurrentWidget(startBar);

    // ---------- 切换按钮行为 ----------
    connect(btnStart, &QPushButton::clicked, this, [=]() {
        toolBarContainer->setCurrentWidget(startBar);
        });
    connect(btnArrange, &QPushButton::clicked, this, [=]() {
        toolBarContainer->setCurrentWidget(arrangeBar);
        });
    connect(btnPage, &QPushButton::clicked, this, [=]() {
        toolBarContainer->setCurrentWidget(pageBar);
        });

    // ---------- 绘图区 ----------
    ShapeLibrary* shapePanel = new ShapeLibrary(this);
    DrawingArea* drawArea = new DrawingArea(this);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(shapePanel);
    splitter->addWidget(drawArea);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({ 150, 650 });

    setCentralWidget(splitter);
    resize(800, 600);

    // ---------- 工具栏按钮与层级控制连接 ----------
    connect(arrangeBar->findChild<QPushButton*>("上移"), &QPushButton::clicked, this, [=]() 
        {
        Shape* selectedShape = drawArea->getSelectedShape();  // 获取当前选中的形状
        if (selectedShape) 
        {
            drawArea->moveUp(selectedShape);
        }
        });
    connect(arrangeBar->findChild<QPushButton*>("下移"), &QPushButton::clicked, this, [=]() 
        {
        Shape* selectedShape = drawArea->getSelectedShape();  // 获取当前选中的形状
        if (selectedShape) 
        {
            drawArea->moveDown(selectedShape);
        }
        });
    connect(arrangeBar->findChild<QPushButton*>("置顶"), &QPushButton::clicked, this, [=]() 
        {
        Shape* selectedShape = drawArea->getSelectedShape();  // 获取当前选中的形状
        if (selectedShape) 
        {
            drawArea->moveToTop(selectedShape);
        }
        });
    connect(arrangeBar->findChild<QPushButton*>("置底"), &QPushButton::clicked, this, [=]() 
        {
        Shape* selectedShape = drawArea->getSelectedShape();  // 获取当前选中的形状
        if (selectedShape) 
        {
            drawArea->moveToBottom(selectedShape);
        }
        });
}

mainwindow::~mainwindow() {}

QWidget* mainwindow::createToolBar(const QStringList& actions)
{
    QWidget* toolbar = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(toolbar);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addStretch();

    for (const QString& text : actions) {
        QPushButton* btn = new QPushButton(text);
        btn->setObjectName(text);  // 为按钮设置对象名称，便于后续查找
        layout->addWidget(btn);
    }

    layout->addStretch();
    return toolbar;
}