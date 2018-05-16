#ifndef D_EXIT_H
#define D_EXIT_H

#include <QDialog>

namespace Ui {
class d_exit;
}

class d_exit : public QDialog
{
    Q_OBJECT

public:
    explicit d_exit(QWidget *parent = 0);
    ~d_exit();

private:
    Ui::d_exit *ui;
};

#endif // D_EXIT_H
