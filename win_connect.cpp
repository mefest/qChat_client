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

void win_connect::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->standardButton(button)== QDialogButtonBox::Ok)
        emit connectToServer(ui->ln_name->text(),
                             ui->ln_addr->text(),
                             ui->ln_port->text(),
                             ui->te_key->toPlainText());
}
