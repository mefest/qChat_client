#include "win_connect.h"
#include "ui_win_connect.h"
#include "saveload.h"

win_connect::win_connect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::win_connect)
{
    ui->setupUi(this);
    saveLoad *load=new saveLoad(this);
    QString name, server,port,key;
    load->loadS(&name,&server,&port,&key);
    delete load;
    ui->ln_name->setText(name);
    ui->ln_addr->setText(server);
    ui->ln_port->setText(port);
    ui->te_key->setText(key);

}

win_connect::~win_connect()
{
    delete ui;

}

void win_connect::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->standardButton(button)== QDialogButtonBox::Ok)
    {
        saveLoad *save=new saveLoad(this);
        save->saveS(ui->ln_name->text(),
                    ui->ln_addr->text(),
                    ui->ln_port->text(),
                    ui->te_key->toPlainText());
        delete save;

        emit connectToServer(ui->ln_name->text(),
                             ui->ln_addr->text(),
                             ui->ln_port->text(),
                             ui->te_key->toPlainText());
    }

}
