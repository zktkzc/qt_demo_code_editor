#include "myhighlighter.h"

MyHighlighter::MyHighlighter(QTextDocument* parent) : QSyntaxHighlighter(parent)
{
    // 普通文本高亮
    addNormalTextFormat();

    // 数字高亮
    addNumberFormat();

    // 字符串高亮
    addStringFormat();

    // 注释高亮
    addCommentFormat();
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
    normalTextFormat.setForeground(QColor(0, 0, 0));

    rule.format = normalTextFormat;

    m_highlightRules.append(rule);
}

void MyHighlighter::addNumberFormat()
{
    HighlightRule rule;
    rule.pattern = QRegExp("\\b\\d+|\\d+\\.\\d+\\b");
    QTextCharFormat numberFormat;

    numberFormat.setFont(QFont(m_fontFamily, m_fontSize));
    numberFormat.setForeground(QColor(250, 80, 50));

    rule.format = numberFormat;

    m_highlightRules.append(rule);
}

void MyHighlighter::addStringFormat()
{
    QTextCharFormat stringFormat;
    stringFormat.setFont(QFont(m_fontFamily, m_fontSize));
    stringFormat.setForeground(QColor(0, 180, 180));

    HighlightRule rule;
    rule.pattern = QRegExp("'[^']*'");
    rule.format = stringFormat;

    m_highlightRules.append(rule);

    rule.pattern = QRegExp("\"[^\"]*\"");
    m_highlightRules.append(rule);

    rule.pattern = QRegExp("`[^`]*`");
    m_highlightRules.append(rule);
}

void MyHighlighter::addCommentFormat()
{
    QTextCharFormat commentFormat;
    commentFormat.setFont(QFont(m_fontFamily, m_fontSize));
    commentFormat.setForeground(Qt::darkGreen);

    HighlightRule rule;
    rule.pattern = QRegExp("\\/\\/.*$");
    rule.format = commentFormat;

    m_highlightRules.append(rule);

    rule.pattern = QRegExp("\\/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*\\/");
    m_highlightRules.append(rule);
}












