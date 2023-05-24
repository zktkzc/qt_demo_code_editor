#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFontDialog>
#include <QSettings>
#include <QList>
#include <QSet>
#include <QDebug>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif
#include <QPrinter>
#endif
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
#if !QT_CONFIG(printer)
    ui->print->setEnabled(false);
#endif

private:
    void InitMenu();
    void OpenRecentFile();

private slots:
    void on_new_file_triggered();

    void on_open_file_triggered();

    void on_save_file_triggered();

    void on_save_as_triggered();

    void on_paste_triggered();

    void on_cut_triggered();

    void on_copy_triggered();

    void on_bolder_triggered(bool checked);

    void on_italic_triggered(bool checked);

    void on_underline_triggered(bool checked);

    void on_font_triggered();

    void on_about_triggered();

    void on_undo_triggered();

    void on_redo_triggered();

    void on_exit_triggered();

    void on_print_triggered();

    void on_clear_history_triggered();

private:
    Ui::MainWindow *ui;
    QString m_currentFile;

};
#endif // MAINWINDOW_H
