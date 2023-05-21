#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QWidget>

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
    Ui::MyTextEdit *ui;
};

#endif // MYTEXTEDIT_H
