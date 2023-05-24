#include "mycodeeditor.h"
#include "myhighlighter.h"

#include <QPainter>

MyCodeEditor::MyCodeEditor(QWidget *parent)
    : QPlainTextEdit{parent}, lineNumberWidget(new LineNumberWidget(this))
{
    initFont();

    initConnection();

    initHighlighter();

    updateLineNumberWidgetWidth();
}

void MyCodeEditor::initFont()
{
    this->setFont(QFont("Consolas", 14));
}

void MyCodeEditor::initConnection()
{
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberWidget(QRect, int)));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberWidgetWidth()));
}

void MyCodeEditor::initHighlighter()
{
    new MyHighlighter(document());
}

int MyCodeEditor::getLineNumberWidgetWidth()
{
    // 获取合适的宽度
    return 8 + QString::number(blockCount() + 1).length() * fontMetrics().horizontalAdvance(QChar('0'));
}

void MyCodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> selections;
    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(QColor(0, 100, 100, 20));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true); // 整行选中
    selection.cursor = textCursor();
    selections.append(selection);
    setExtraSelections(selections);
}

void MyCodeEditor::updateLineNumberWidget(QRect rect, int dy)
{
    if (dy)
        lineNumberWidget->scroll(0, dy);
    else
        lineNumberWidget->update(0, rect.y(), getLineNumberWidgetWidth(), rect.height());
}

void MyCodeEditor::updateLineNumberWidgetWidth()
{
    setViewportMargins(getLineNumberWidgetWidth(), 0, 0, 0); // 设置边距
}

void MyCodeEditor::lineNumberWidgetPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberWidget);
    painter.fillRect(event->rect(), QColor(100, 100, 100, 20));
    auto block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    // 拿到当前block的top
    int cursorTop = blockBoundingGeometry(textCursor().block()).translated(contentOffset()).top();
    int top = blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        painter.setPen(cursorTop == top ? Qt::black : Qt::gray);
        painter.drawText(0, top, getLineNumberWidgetWidth(), bottom - top, Qt::AlignRight, QString::number(blockNumber + 1));
        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        blockNumber++;
    }
}

void MyCodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    lineNumberWidget->setGeometry(0, 0, getLineNumberWidgetWidth(), contentsRect().height());
}










