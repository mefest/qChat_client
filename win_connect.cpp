#include "win_connect.h"
#include "ui_win_connect.h"

win_connect::win_connect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::win_connect)
{
    ui->setupUi(this);
}

win_connect::~win_connect()
{
    delete ui;
}
