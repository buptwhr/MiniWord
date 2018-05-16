#ifndef DIALOGOPEN_H
#define DIALOGOPEN_H

#include <QDialog>
#include <QFileDialog>
namespace Ui {
class DialogOpen;
}

class DialogOpen : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOpen(QWidget *parent = 0);
    ~DialogOpen();

signals:
    void sendfilename(QString);

private slots:
    void on_pushButton_clicked();


    void on_buttonBox_accepted();

private:
    Ui::DialogOpen *ui;
};

#endif // DIALOGOPEN_H
