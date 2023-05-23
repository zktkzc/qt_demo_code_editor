#ifndef MYHIGHLIGHTER_H
#define MYHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextStream>

class MyHighlighter : public QSyntaxHighlighter
{
public:
    explicit MyHighlighter(QTextDocument* parent = nullptr);

protected:
    void highlightBlock(const QString &text);

private:
    void addNormalTextFormat();
    void addNumberFormat();
    void addStringFormat();
    void addCommentFormat();
    void addMultiLineCommentFormat(const QString &text);
    void addKeywordsFormat();
    void addClassNameFormat();

private:
    struct HighlightRule{
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightRule> m_highlightRules;
    QString m_fontFamily = "Consolas";
    int m_fontSize = 14;
};

#endif // MYHIGHLIGHTER_H
