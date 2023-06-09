#include "myhighlighter.h"
#include "mytexteditbycode.h"

MyTextEditByCode::MyTextEditByCode(QWidget *parent)
    : QWidget{parent}
{
    // 初始化UI组件
    initWidget();

    // 初始化字体
    initFont();

    // 绑定
    initConnection();

    // 初始化高亮
    initHighlighter();

    // 当前行高亮
    highlightCurrentLine();
}

MyTextEditByCode::~MyTextEditByCode()
{
    delete textEdit;
    delete textBrowser;
    delete scrollBar;
}

void MyTextEditByCode::initWidget()
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

    textEdit = new QTextEdit();
    textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEdit->setLineWrapMode(QTextEdit::NoWrap);

    textBrowser = new QTextBrowser();
    textBrowser->setMaximumWidth(25);
    textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollBar = new QScrollBar();
    scrollBar->setOrientation(Qt::Horizontal); // 设置滚动条轴向

    horizontalLayout->addWidget(textBrowser);
    horizontalLayout->addWidget(textEdit);

    verticalLayout->addWidget(horizontalLayoutWidget);
    verticalLayout->addWidget(scrollBar);
}

void MyTextEditByCode::initFont()
{
    m_font = QFont("Consolas", 14);
    textEdit->setFont(m_font);

    QTextBlockFormat format;
    format.setLineHeight(QFontMetrics(m_font).height() * 1.1, QTextBlockFormat::FixedHeight);
    auto cursor = textEdit->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);

    textBrowser->setFont(m_font);
}

void MyTextEditByCode::initConnection()
{
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));

    connect(textEdit->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onTextEditHScrollBarChanged()));
    connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(onScrollBarChanged()));

    connect(textEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onTextEditVScrollBarChanged()));
    connect(textBrowser->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onTextBrowserVerticalScrollBarChanged()));

    connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
}

void MyTextEditByCode::initHighlighter()
{
    new MyHighlighter(textEdit->document());
}

void MyTextEditByCode::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> selections;
    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(QColor(0, 100, 100, 20));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true); // 整行选中
    selection.cursor = textEdit->textCursor();
    // selection.cursor.clearSelection();
    selections.append(selection);

    textEdit->setExtraSelections(selections);
}

void MyTextEditByCode::onTextEditHScrollBarChanged()
{
    scrollBar->setMaximum(textEdit->horizontalScrollBar()->maximum());
    scrollBar->setMinimum(textEdit->horizontalScrollBar()->minimum());
    scrollBar->setPageStep(textEdit->horizontalScrollBar()->pageStep());
    scrollBar->setValue(textEdit->horizontalScrollBar()->value());
}

void MyTextEditByCode::onScrollBarChanged()
{
    textEdit->horizontalScrollBar()->setValue(scrollBar->value());
}

void MyTextEditByCode::onTextEditVScrollBarChanged()
{
    textBrowser->verticalScrollBar()->setMaximum(textEdit->verticalScrollBar()->maximum());
    textBrowser->verticalScrollBar()->setMinimum(textEdit->verticalScrollBar()->minimum());
    textBrowser->verticalScrollBar()->setPageStep(textEdit->verticalScrollBar()->pageStep());
    textBrowser->verticalScrollBar()->setValue(textEdit->verticalScrollBar()->value());
}

void MyTextEditByCode::onTextBrowserVerticalScrollBarChanged()
{
    textEdit->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->value());
}

void MyTextEditByCode::onTextChanged()
{
    // 行数
    int lineCountOfTextEdit = textEdit->document()->lineCount();
    QString text = textBrowser->toPlainText();
    int lineCountOfTextBrowser = text.trimmed().split("\n").length();

    if (lineCountOfTextBrowser > lineCountOfTextEdit)
    {
        for (int i = lineCountOfTextBrowser; i > lineCountOfTextEdit; i--)
        {
            text.chop((QString::number(i) + "\n").length());
        }
    }
    else if (lineCountOfTextBrowser == 1 && text.length() < 1)
    {
        text += "1\n";
    }
    else if (lineCountOfTextBrowser < lineCountOfTextEdit)
    {
        for (int i = lineCountOfTextBrowser; i < lineCountOfTextEdit; i++)
        {
            text += QString::number(i+1) + "\n";
        }
    }

    textBrowser->setMaximumWidth(25 + QString::number(lineCountOfTextEdit).length() * 7);
    textBrowser->setText(text);

    QTextBlockFormat format;
    format.setLineHeight(QFontMetrics(m_font).height() * 1.1, QTextBlockFormat::FixedHeight);
    format.setAlignment(Qt::AlignRight);
    auto cursor = textBrowser->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
}
