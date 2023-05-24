#include "mycodeeditor.h"
#include "qscrollbar.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>

MyCodeEditor::MyCodeEditor(QWidget *parent, QFont font)
    : QPlainTextEdit{parent}, lineNumberWidget(new LineNumberWidget(this))
{
    initConnection();

    initHighlighter();

    setAllFont(font);

    updateLineNumberWidgetWidth();

    setLineWrapMode(QPlainTextEdit::NoWrap);
}

MyCodeEditor::~MyCodeEditor()
{
    if (lineNumberWidget)
    {
        delete lineNumberWidget;
        lineNumberWidget = nullptr;
    }

    if (m_highlighter)
    {
        delete m_highlighter;
        m_highlighter = nullptr;
    }
}

void MyCodeEditor::initConnection()
{
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberWidget(QRect, int)));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberWidgetWidth()));
}

void MyCodeEditor::initHighlighter()
{
    m_highlighter = new MyHighlighter(document());
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
        painter.drawText(0, top, getLineNumberWidgetWidth() - 3, bottom - top, Qt::AlignRight, QString::number(blockNumber + 1));
        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        blockNumber++;
    }
}

void MyCodeEditor::lineNumberWidgetMousePressEvent(QMouseEvent *event)
{
    setTextCursor(QTextCursor(document()->findBlockByLineNumber(event->y() / fontMetrics().height() + verticalScrollBar()->value())));
}

void MyCodeEditor::lineNumberWidgetWheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Horizontal)
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - event->delta());
    else
        verticalScrollBar()->setValue(verticalScrollBar()->value() - event->delta());
    event->accept();
}

bool MyCodeEditor::saveFile()
{
    QString fileName;
    if (m_fileName.isEmpty())
    {
        fileName = QFileDialog::getSaveFileName(this, "保存文件");
        m_fileName = fileName;
    }

    fileName = m_fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "警告", "无法保存文件：" + file.errorString());
        return false;
    }

    QTextStream out(&file);
    out << toPlainText();
    file.close();

    return true;
}

bool MyCodeEditor::saveAs()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, "另存文件");
    m_fileName = fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "警告", "无法保存文件：" + file.errorString());
        return false;
    }

    QTextStream out(&file);
    QString text = toPlainText();
    out << text;
    file.close();
    return true;
}

void MyCodeEditor::setAllFont(QFont font)
{
    this->setFont(font);
    m_highlighter->setFont(font);
    updateLineNumberWidgetWidth();
}

void MyCodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    lineNumberWidget->setGeometry(0, 0, getLineNumberWidgetWidth(), contentsRect().height());
}










