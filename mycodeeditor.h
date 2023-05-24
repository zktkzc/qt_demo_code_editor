#ifndef MYCODEEDITOR_H
#define MYCODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>

class LineNumberWidget;

class MyCodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit MyCodeEditor(QWidget *parent = nullptr);
    ~MyCodeEditor();
    void lineNumberWidgetPaintEvent(QPaintEvent* event);
    void lineNumberWidgetMousePressEvent(QMouseEvent* event);
    void lineNumberWidgetWheelEvent(QWheelEvent* event);
    bool saveFile();
    bool saveAs();
    void setFileName(QString fileName)
    {
        m_fileName = fileName;
    }
    QString getFileName()
    {
        return m_fileName;
    }

protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    void initFont();
    void initConnection();
    void initHighlighter();
    int getLineNumberWidgetWidth();

private slots:
    void highlightCurrentLine();
    void updateLineNumberWidget(QRect rect, int dy);
    void updateLineNumberWidgetWidth();

private:
    LineNumberWidget* lineNumberWidget;
    QString m_fileName;

};

class LineNumberWidget : public QWidget
{
public:
    explicit LineNumberWidget(MyCodeEditor* editor = nullptr) : QWidget(editor)
    {
        codeEditor = editor;
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        codeEditor->lineNumberWidgetPaintEvent(event);
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        codeEditor->lineNumberWidgetMousePressEvent(event);
    }

    void wheelEvent(QWheelEvent* event) override
    {
        codeEditor->lineNumberWidgetWheelEvent(event);
    }

private:
    MyCodeEditor* codeEditor;
};

#endif // MYCODEEDITOR_H