#include "mainwindow.h"
#include "ui_mainwindow.h"

QSettings *m_settings;

// 获取历史记录
QList<QString> GetHistory()
{
    // 打开开始读
    int size = m_settings->beginReadArray("history");

    QList<QString> list;
    for (int i = 0; i < size; i++)
    {
        m_settings->setArrayIndex(i);
        list.append(m_settings->value("path").toString());
    }
    // 关闭开始读
    m_settings->endArray();

    return list;
}

// 保存历史记录
void SaveHistory(QString path)
{
    // 获取历史记录
    auto list = GetHistory();

    // 去重
    foreach (auto str, list) {
        if (str == path) list.removeOne(str);
    }
    list.append(path);

    // 打开开始写入
    m_settings->beginWriteArray("history");

    for (int i = 0; i < list.size(); i++)
    {
        m_settings->setArrayIndex(i);

        // 保存字符串
        m_settings->setValue("path", list[i]);
    }

    // 关闭开始写入
    m_settings->endArray();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    if (!m_settings)
        m_settings = new QSettings("settings.ini", QSettings::IniFormat);

    InitMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitMenu()
{
    QMenu *recent = this->findChild<QMenu*>("recent");
    qDebug() << recent->title();
    auto childList = recent->children().toSet();
    foreach (auto ch, childList) {
        QAction* action = (QAction*)ch;
        recent->removeAction(action);
    }

    auto list = GetHistory();
    for (int i = list.size() - 1; i >= 0; i--)
    {
        recent->addAction(list[i], this, &MainWindow::OpenRecentFile);
    }

    if (list.size() > 0)
        recent->addAction("清除历史记录", this, &MainWindow::on_clear_history_triggered, QKeySequence("Ctrl+Alt+Shift+C"));
}

void MainWindow::OpenRecentFile()
{
    QAction* action = (QAction*)sender();
    QString fileName = action->text();
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "警告", "无法打开此文件：" + file.errorString());
        return;
    }
    m_currentFile = fileName;
    this->setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();

    SaveHistory(m_currentFile);
    InitMenu();
}


void MainWindow::on_new_file_triggered()
{
    m_currentFile.clear();
    ui->textEdit->clear();
}


void MainWindow::on_open_file_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "警告", "无法打开此文件：" + file.errorString());
        return;
    }

    m_currentFile = fileName;
    this->setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();

    SaveHistory(m_currentFile);
    InitMenu();
}


void MainWindow::on_save_file_triggered()
{
    QString fileName;
    if (m_currentFile.isEmpty())
    {
        fileName = QFileDialog::getSaveFileName(this, "保存文件");
        m_currentFile = fileName;
    }

    fileName = m_currentFile;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "警告", "无法保存文件：" + file.errorString());
        return;
    }

    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toHtml();
    out << text;
    file.close();
    SaveHistory(m_currentFile);
    InitMenu();
}


void MainWindow::on_save_as_triggered()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, "另存文件");
    m_currentFile = fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "警告", "无法保存文件：" + file.errorString());
        return;
    }

    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toHtml();
    out << text;
    file.close();
    SaveHistory(m_currentFile);
    InitMenu();
}


void MainWindow::on_paste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_cut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_copy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_bolder_triggered(bool checked)
{
    ui->textEdit->setFontWeight(checked ? QFont::Bold : QFont::Normal);
}

void MainWindow::on_italic_triggered(bool checked)
{
    ui->textEdit->setFontItalic(checked);
}


void MainWindow::on_underline_triggered(bool checked)
{
    ui->textEdit->setFontUnderline(checked);
}


void MainWindow::on_font_triggered()
{
    bool fontSelected;
    auto font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected) ui->textEdit->setFont(font);
}


void MainWindow::on_about_triggered()
{
    QMessageBox::about(this, "这是我的Notepad！", "这是我的Notepad，欢迎学习和使用！");
}


void MainWindow::on_undo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_redo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_exit_triggered()
{
    QCoreApplication::exit();
}


void MainWindow::on_print_triggered()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
    QPrinter printDev;
#if QT_CONFIG(printdialog)
    QPrintDialog dialog(&printDev, this);
    if (dialog.exec() == QDialog::Rejected) return;
#endif
    ui->textEdit->print(&printDev);
#endif
}

// 清空历史记录
void MainWindow::on_clear_history_triggered()
{
    m_settings->remove("history");
    InitMenu();
}



