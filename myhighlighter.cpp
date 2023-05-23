#include "myhighlighter.h"

MyHighlighter::MyHighlighter(QTextDocument* parent) : QSyntaxHighlighter(parent)
{
    // 普通文本高亮
    addNormalTextFormat();
}

void MyHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightRule &rule, m_highlightRules) {
        QRegExp regExp(rule.pattern);
        int index = regExp.indexIn(text);
        while (index >= 0)
        {
            int length = regExp.matchedLength();
            setFormat(index, length, rule.format);
            index = regExp.indexIn(text, index + length);
        }
    }
}

void MyHighlighter::addNormalTextFormat()
{
    HighlightRule rule;
    rule.pattern = QRegExp("[a-z0-9A-Z]+");
    QTextCharFormat normalTextFormat;

    normalTextFormat.setFont(QFont(m_fontFamily, m_fontSize));
    normalTextFormat.setForeground(QColor(200, 0, 0));

    rule.format = normalTextFormat;

    m_highlightRules.append(rule);
}
