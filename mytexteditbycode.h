#ifndef MYTEXTEDITBYCODE_H
#define MYTEXTEDITBYCODE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QTextBrowser>
#include <QScrollBar>

class MyTextEditByCode : public QWidget
{
    Q_OBJECT
public:
    explicit MyTextEditByCode(QWidget *parent = nullptr);

private:
    void initWidget();
    void initFont();
    void initConnection();
    void initHighlighter();

private:
    QTextEdit* textEdit;
    QTextBrowser* textBrowser;
    QScrollBar* scrollBar;
    QFont m_font;

private slots:
    void onTextEditHScrollBarChanged();
    void onScrollBarChanged();
    void onTextEditVScrollBarChanged();
    void onTextBrowserVerticalScrollBarChanged();
    void onTextChanged();
    void highlightCurrentLine();
};

#endif // MYTEXTEDITBYCODE_H
