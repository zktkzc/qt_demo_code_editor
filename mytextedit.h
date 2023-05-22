#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>

namespace Ui {
class MyTextEdit;
}

class MyTextEdit : public QWidget
{
    Q_OBJECT

public:
    explicit MyTextEdit(QWidget *parent = nullptr);
    ~MyTextEdit();

private:
    void InitConnect();
    void InitFont();

private slots:
    void TextEditHScrollBarChanged();
    void ScrollBarChanged();
    void TextEditVScrollBarChanged();
    void TextBrowserVerticalScrollBarChanged();
    void OnTextChanged();

private:
    Ui::MyTextEdit *ui;
};

#endif // MYTEXTEDIT_H
