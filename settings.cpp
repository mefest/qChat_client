#include "settings.h"
#include "ui_settings.h"
#include <QColorDialog>
#include <QFontDialog>
#include "saveload.h"
#include <QDebug>

settings::settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    myNick=new QColor();
    otherNick=new QColor();
    font= new QFont();
    saveLoad *load =new saveLoad(this);

    load->load(myNick,otherNick,font);

    ui->pb_myNick->setStyleSheet("QPushButton { background-color: rgb("+QString::number(myNick->red())+
                                 ","+QString::number(myNick->green())+","+QString::number(myNick->blue())+");}");


    ui->pb_otherNick->setStyleSheet("QPushButton { background-color: rgb("+QString::number(otherNick->red())+
                                    ","+QString::number(otherNick->green())+","+QString::number(otherNick->blue())+");}");
    delete load;
}

settings::~settings()
{
    delete ui;
}

void settings::on_pb_myNick_clicked()
{
    QColor color;
    color= QColorDialog::getColor(*myNick,this);
    if(color.isValid())
    {
        *myNick=color;
        ui->pb_myNick->setStyleSheet("QPushButton { background-color: rgb("+QString::number(myNick->red())+
                                     ","+QString::number(myNick->green())+","+QString::number(myNick->blue())+");}");
    }
}

void settings::on_pb_otherNick_clicked()
{
    QColor color;
    color= QColorDialog::getColor(*otherNick,this);
    if(color.isValid())
    {
        *otherNick=color;
    ui->pb_otherNick->setStyleSheet("QPushButton { background-color: rgb("+QString::number(otherNick->red())+
                                    ","+QString::number(otherNick->green())+","+QString::number(otherNick->blue())+");}");
    }
}

void settings::on_pb_font_clicked()
{
    bool ok;

    *font = QFontDialog::getFont(&ok, *font,this,"Выберите шрифт");
    if (ok) {

        //  text->setFont(font);
    } else{
        // the user canceled the dialog; font is set to the initial
        // value, in this case Times, 12.
    }
}

void settings::on_buttonBox_accepted()
{
    qDebug()<<"Hажата ок";
    saveLoad *save =new saveLoad(this);
    save->save(myNick,otherNick,font);
}
