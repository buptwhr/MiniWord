#ifndef D_NEW_H
#define D_NEW_H

#include <QDialog>

namespace Ui {
class d_new;
}

class d_new : public QDialog
{
    Q_OBJECT

public:
    explicit d_new(QWidget *parent = 0);
    ~d_new();

signals:
    void sendfilename(QString);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::d_new *ui;
};

#endif // D_NEW_H
