#include "mytexteditbycode.h"

MyTextEditByCode::MyTextEditByCode(QWidget *parent)
    : QWidget{parent}
{
    QWidget* horizontalLayoutWidget = new QWidget();

    // 竖向布局
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(0); // 取消边距
    verticalLayout->setMargin(0);

    // 横向布局
    QHBoxLayout* horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
    horizontalLayout->setMargin(0);
    horizontalLayout->setSpacing(0);

    QTextEdit* textEdit = new QTextEdit();
    textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QTextBrowser* textBrowser = new QTextBrowser();
    textBrowser->setMaximumWidth(25);
    textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QScrollBar* scrollBar = new QScrollBar();
    scrollBar->setOrientation(Qt::Horizontal); // 设置滚动条轴向

    horizontalLayout->addWidget(textBrowser);
    horizontalLayout->addWidget(textEdit);

    verticalLayout->addWidget(horizontalLayoutWidget);
    verticalLayout->addWidget(scrollBar);
}
