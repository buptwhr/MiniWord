#ifndef D_SAVE_H
#define D_SAVE_H

#include <QDialog>

namespace Ui {
class d_save;
}

class d_save : public QDialog
{
    Q_OBJECT

public:
    explicit d_save(QWidget *parent = 0);
    ~d_save();
    void lineedit_settext(QString);
private slots:
    void on_buttonBox_accepted();
signals:
    void sendfilename(QString);
private:
    Ui::d_save *ui;
};

#endif // D_SAVE_H
