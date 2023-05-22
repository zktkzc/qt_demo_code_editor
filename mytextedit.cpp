#include "mytextedit.h"
#include "ui_mytextedit.h"

MyTextEdit::MyTextEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyTextEdit)
{
    ui->setupUi(this);

    // 绑定滚动条
    InitConnect();

    // 初始化字体
    InitFont();
}

MyTextEdit::~MyTextEdit()
{
    delete ui;
}

void MyTextEdit::InitConnect()
{
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));

    connect(ui->textEdit->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(TextEditHScrollBarChanged()));
    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(ScrollBarChanged()));

    connect(ui->textEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(TextEditVScrollBarChanged()));
    connect(ui->textBrowser->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(TextBrowserVerticalScrollBarChanged()));
}

void MyTextEdit::InitFont()
{
    QFont font("新宋体", 14);
    ui->textEdit->setFont(font);
    ui->textBrowser->setFont(font);
}

void MyTextEdit::TextEditHScrollBarChanged()
{
    ui->horizontalScrollBar->setMaximum(ui->textEdit->horizontalScrollBar()->maximum());
    ui->horizontalScrollBar->setMinimum(ui->textEdit->horizontalScrollBar()->minimum());
    ui->horizontalScrollBar->setPageStep(ui->textEdit->horizontalScrollBar()->pageStep());
    ui->horizontalScrollBar->setValue(ui->textEdit->horizontalScrollBar()->value());
}

void MyTextEdit::ScrollBarChanged()
{
    ui->textEdit->horizontalScrollBar()->setValue(ui->horizontalScrollBar->value());
}

void MyTextEdit::TextEditVScrollBarChanged()
{
    ui->textBrowser->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->value());
}

void MyTextEdit::TextBrowserVerticalScrollBarChanged()
{
    ui->textEdit->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->value());
}

void MyTextEdit::OnTextChanged()
{
    // 行数
    int lineCount = ui->textEdit->document()->lineCount();
    QString text = "";
    for (int i = 0; i < lineCount; i++)
    {
        text += QString::number(i+1) + "\n";
    }

    ui->textBrowser->setMaximumWidth(25 + QString::number(lineCount).length() * 7);

    ui->textBrowser->setText(text);
}


