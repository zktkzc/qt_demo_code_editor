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

private slots:
    void onTextEditHScrollBarChanged();
    void onScrollBarChanged();
    void onTextEditVScrollBarChanged();
    void onTextBrowserVerticalScrollBarChanged();
    void onTextChanged();

};

#endif // MYTEXTEDITBYCODE_H