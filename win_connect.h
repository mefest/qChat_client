#ifndef WIN_CONNECT_H
#define WIN_CONNECT_H

#include <QDialog>

namespace Ui {
class win_connect;
}

class win_connect : public QDialog
{
    Q_OBJECT

public:
    explicit win_connect(QWidget *parent = 0);
    ~win_connect();

private:
    Ui::win_connect *ui;
};

#endif // WIN_CONNECT_H
