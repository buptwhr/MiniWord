#ifndef D_INSERT_H
#define D_INSERT_H

#include <QDialog>

namespace Ui {
class d_insert;
}

class d_insert : public QDialog
{
    Q_OBJECT

public:
    explicit d_insert(QWidget *parent = 0);
    ~d_insert();

private:
    Ui::d_insert *ui;
};

#endif // D_INSERT_H
