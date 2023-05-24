#include "mainwindow.h"
#include "mycodeeditor.h"
#include "ui_mainwindow.h"

// 获取历史记录
QList<QString> MainWindow::GetHistory()
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
void MainWindow::SaveHistory(QString path)
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

void MainWindow::initAction()
{
    bool valid = ui->tabWidget->count() > 0;
    ui->save_file->setEnabled(valid);
    ui->save_as->setEnabled(valid);
    ui->copy->setEnabled(valid);
    ui->paste->setEnabled(valid);
    ui->cut->setEnabled(valid);
    ui->undo->setEnabled(valid);
    ui->redo->setEnabled(valid);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->tabWidget);
    m_settings = new QSettings("settings.ini", QSettings::IniFormat);

    initFont();

    InitMenu();

    initAction();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    MyCodeEditor* codeEditor = (MyCodeEditor*)(ui->tabWidget->currentWidget());
    if (ui->tabWidget->count() > 0)
    {
        QMessageBox::question(this, "警告", "您还有未保存文档！确定要关闭吗？", QMessageBox::Yes | QMessageBox::No) ==
            QMessageBox::Yes ? event->accept() : event->ignore();
    }
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
    this->setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    MyCodeEditor* codeEditor = new MyCodeEditor(this, QFont(m_fontFamily, m_fontSize));
    codeEditor->setPlainText(text);
    codeEditor->setFileName(fileName);
    ui->tabWidget->addTab(codeEditor, fileName);
    initAction();
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    file.close();

    SaveHistory(fileName);
    InitMenu();
}

void MainWindow::initFont()
{
    m_fontFamily = m_settings->value("font_family", "Consolas").toString();
    m_fontSize = m_settings->value("font_size", 14).toInt();
}


void MainWindow::on_new_file_triggered()
{
    ui->tabWidget->addTab(new MyCodeEditor(this, QFont(m_fontFamily, m_fontSize)), "NewTab.txt");
    initAction();
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

    this->setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    MyCodeEditor* codeEditor = new MyCodeEditor(this, QFont(m_fontFamily, m_fontSize));
    codeEditor->setPlainText(text);
    codeEditor->setFileName(fileName);
    ui->tabWidget->addTab(codeEditor, fileName);
    initAction();
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    file.close();

    SaveHistory(fileName);
    InitMenu();
}


void MainWindow::on_save_file_triggered()
{
    MyCodeEditor* codeEditor = (MyCodeEditor*)(ui->tabWidget->currentWidget());
    if (codeEditor)
    {
        if (codeEditor->saveFile())
        {
            SaveHistory(codeEditor->getFileName());
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), codeEditor->getFileName());
            InitMenu();
        }
    }
}


void MainWindow::on_save_as_triggered()
{
    MyCodeEditor* codeEditor = (MyCodeEditor*)(ui->tabWidget->currentWidget());
    if (codeEditor)
    {
        if (codeEditor->saveAs())
        {
            SaveHistory(codeEditor->getFileName());
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), codeEditor->getFileName());
            InitMenu();
        }
    }
}


void MainWindow::on_paste_triggered()
{
    MyCodeEditor* codeEditor = (MyCodeEditor*)(ui->tabWidget->currentWidget());
    if (codeEditor)
    {
        codeEditor->paste();
    }
}


void MainWindow::on_cut_triggered()
{
    MyCodeEditor* codeEditor = (MyCodeEditor*)(ui->tabWidget->currentWidget());
    if (codeEditor)
    {
        codeEditor->cut();
    }
}


void MainWindow::on_copy_triggered()
{
    MyCodeEditor* codeEditor = (MyCodeEditor*)(ui->tabWidget->currentWidget());
    if (codeEditor)
    {
        codeEditor->copy();
    }
}

void MainWindow::on_about_triggered()
{
    QMessageBox::about(this, "这是我的CodeEditor！", "这是我的CodeEditor，欢迎学习和使用！");
}


void MainWindow::on_undo_triggered()
{
    MyCodeEditor* codeEditor = (MyCodeEditor*)(ui->tabWidget->currentWidget());
    if (codeEditor)
    {
        codeEditor->undo();
    }
}


void MainWindow::on_redo_triggered()
{
    MyCodeEditor* codeEditor = (MyCodeEditor*)(ui->tabWidget->currentWidget());
    if (codeEditor)
    {
        codeEditor->redo();
    }
}


void MainWindow::on_exit_triggered()
{
    QCoreApplication::exit();
}

void MainWindow::on_print_triggered()
{
    MyCodeEditor* codeEditor = (MyCodeEditor*)(ui->tabWidget->currentWidget());
    if (codeEditor)
    {
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
    QPrinter printDev;
#if QT_CONFIG(printdialog)
    QPrintDialog dialog(&printDev, this);
    if (dialog.exec() == QDialog::Rejected) return;
#endif
        codeEditor->print(&printDev);
#endif
    }
}

// 清空历史记录
void MainWindow::on_clear_history_triggered()
{
    m_settings->remove("history");
    InitMenu();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    MyCodeEditor* codeEditor = (MyCodeEditor*)(ui->tabWidget->currentWidget());
    if (!codeEditor->checkSaved())
    {
        auto btn = QMessageBox::question(this, "警告", "您还没有保存文档！是否保存？", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (btn == QMessageBox::Yes)
        {
            if (codeEditor->saveFile())
            {
                SaveHistory(codeEditor->getFileName());
                ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), codeEditor->getFileName());
                InitMenu();
            }
            return;
        }
        else if (btn == QMessageBox::Cancel)
        {
            return;
        }
    }

    delete codeEditor;
    ui->tabWidget->removeTab(index);
    initAction();
}


void MainWindow::on_font_triggered()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, QFont(m_fontFamily, m_fontSize),this);
    MyCodeEditor* codeEditor = (MyCodeEditor*)(ui->tabWidget->currentWidget());
    if (fontSelected)
    {
        codeEditor->setAllFont(font);
    }
    m_settings->setValue("font_family", font.family());
    m_settings->setValue("font_size", font.pointSize());
}

