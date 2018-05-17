#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMessageBox>
#include <QMainWindow>
#include <dialogopen.h>
#include <d_replace.h>
#include <d_new.h>
#include <d_save.h>
#include <datastruct.h>
#include <QMouseEvent>
#include <QTimer>
#include <QScrollBar>
#include <QTime>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_actionOpen_triggered();
    void receivefilename(QString filename);

    void on_actionreplace_triggered();
    void f_replace(QString s1,QString s2);

    void graphic_datastruct();
    void wheelEvent(QWheelEvent *event);
    void on_actionexit_triggered();





    void on_actionNew_triggered();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;
    Text* t;//主数据结构的指针
    QLabel **label;
    int labelnum;
    int state;
protected:
  //  void mouseMoveEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
