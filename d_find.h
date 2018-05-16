#ifndef D_FIND_H
#define D_FIND_H

#include <QDialog>

namespace Ui {
class d_find;
}

class d_find : public QDialog
{
    Q_OBJECT

public:
    explicit d_find(QWidget *parent = 0);
    ~d_find();

private:
    Ui::d_find *ui;
};

#endif // D_FIND_H
