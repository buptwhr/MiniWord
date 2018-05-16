#ifndef D_REPLACE_H
#define D_REPLACE_H

#include <QDialog>

namespace Ui {
class d_replace;
}

class d_replace : public QDialog
{
    Q_OBJECT

public:
    explicit d_replace(QWidget *parent = 0);
    ~d_replace();
signals:
    void send_replace_message(QString,QString);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::d_replace *ui;
};

#endif // D_REPLACE_H
