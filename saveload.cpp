#include "saveload.h"
#include <QColor>
#include <QMessageBox>
#include <QFile>
#include <QDebug>


saveLoad::saveLoad(QObject *parent) :
    QObject(parent)
{
    _my = new QColor();
    _other= new QColor();
    _font =new QFont();
}

void saveLoad::save(QColor *my, QColor *other, QFont *font)
{
    QFile file("settings.dat");
    if (!file.open(QFile::ReadWrite)) { //открываем файл для записи
        //        QMessageBox::warning(this, tr("Application"),
        //                             tr("Cannot read file %1:\n%2.")) // если не получилось то выводим окно
        //                             //.arg("settings.dat")
        //                            // .arg(file.errorString()));


        //        QMessageBox::information(this, tr("QChat Client"),
        //                                 tr("The connection was refused by the peer. "
        //                                    "Убедитесь, что сервер запущен "
        //                                    "и провельте адресс сервера и порт. "
        //                                    "settings are correct."));
        qDebug()<<"не открыли";
        return;
    }



    QDataStream out(&file);
    out<<*my;
    out<<*other;
    out << *font;
    qDebug()<<out.device()->pos();
    file.close();



}

void saveLoad::load(QColor* my, QColor* other , QFont *font)
{
    QFile file("settings.dat");

    if (!file.open(QFile::ReadOnly )) { //открываем файл для чтения
        //        QMessageBox::warning(this, tr("Application"),
        //                             tr("Cannot read file %1:\n%2.") // если не получилось то выводим окно
        //                             .arg("qwe")
        //                             .arg(file.errorString()));
        qDebug()<<"не открыли";
        return;
    }


    QDataStream out(&file);
    out>>*my;
    out>>*other;
    out>>*font;

    file.close();

}

void saveLoad::saveS(QString name, QString server, QString port, QString key)
{
    QFile file("settings.dat");
    if (!file.open(QFile::ReadWrite )) { //открываем файл для записи
        //        QMessageBox::warning(this, tr("Application"),
        //                             tr("Cannot read file %1:\n%2.")) // если не получилось то выводим окно
        //                             //.arg("settings.dat")
        //                            // .arg(file.errorString()));


        //        QMessageBox::information(this, tr("QChat Client"),
        //                                 tr("The connection was refused by the peer. "
        //                                    "Убедитесь, что сервер запущен "
        //                                    "и провельте адресс сервера и порт. "
        //                                    "settings are correct."));
        qDebug()<<"не открыли";
        return;
    }



    QDataStream out(&file);
    out.device()->seek(100);

    out<<name;
    out<<server;
    out<<port;
    out <<key;
    file.close();
}

void saveLoad::loadS(QString *name, QString *server, QString *port, QString *key)
{
    QFile file("settings.dat");

    if (!file.open(QFile::ReadOnly )) { //открываем файл для чтения
        //        QMessageBox::warning(this, tr("Application"),
        //                             tr("Cannot read file %1:\n%2.") // если не получилось то выводим окно
        //                             .arg("qwe")
        //                             .arg(file.errorString()));
        qDebug()<<"не открыли";
        return;
    }



    QDataStream out(&file);
    out.device()->seek(100);
    out>>*name;
    out>>*server;
    out>>*port;
    out>>*key;
    file.close();
}
