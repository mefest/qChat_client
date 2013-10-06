#include <QTcpSocket>
#include <QDebug>
#include <QTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "win_connect.h"
#include "client.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->removeTab(1);
    ui->mainToolBar->hide();
    tcpSocket= new client(this);
    myName =new QColor(Qt::blue);
    otherName =new QColor(Qt::magenta);
    //tcpSocket=new QTcpSocket(this);

    encrypt=false;
    ui->act_crypt->setEnabled(false);
    ctx= new BLOWFISH_CTX;

    connect(tcpSocket,SIGNAL(connected()),this,SLOT(connectSucces()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(inable()));
    connect(tcpSocket,SIGNAL(addUsers(QString)),this,SLOT(addUsersToGui(QString)));
    connect(this,SIGNAL(sendMessage(QString)),tcpSocket,SLOT(sendToServerMessage(QString)));
    connect(tcpSocket,SIGNAL(getMessage(int,QString)),this,SLOT(getMess(int,QString)));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(on_act_disconnect_triggered()));
    connect(tcpSocket,SIGNAL(delUser(QString)),this,SLOT(deleteUser(QString)));
    connect(ui->te_input,SIGNAL(giveText(QString)),this,SLOT(getText(QString)));

    //<--here

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToServer(QString name, QString addrServ, QString portServ, QString key)
{
    if(!tcpSocket->getRuning())
    {
        qDebug()<<"hi"<<addrServ<<portServ;
        if(key.length()>6 && key!="")
        {
            tcpSocket->_encrypt=true;
            Blowfish_Init(ctx,&key,key.length());
            tcpSocket->setCTX(ctx);
            encrypt=true;
            ui->act_crypt->setEnabled(true);
            qDebug()<<"crypt on";
        }
        else
        {
            encrypt=false;
            ui->act_crypt->setEnabled(false);
        }
        tcpSocket->name=name;
        tcpSocket->connectTo(addrServ,(quint16) portServ.toInt());
        //tcpSocket->connectToHost(addrServ,(quint16) portServ.toInt());
        ui->statusBar->showMessage("Подключаемся к "+addrServ);
    }
    else
        qDebug()<<"что то не так!";
}

void MainWindow::messageToGui(int kod, QString name, QString mess)
{
    QTextEdit *out= ui->te_message;
    out->moveCursor(QTextCursor::End);
    out->setTextBackgroundColor(Qt::white);
    out->insertPlainText(QTime::currentTime().toString());
    switch(kod)
    {
    case 1:
        qDebug()<<"system mess";
        out->setTextBackgroundColor(QColor("red"));
        out->setTextColor(QColor(Qt::black));
        out->insertPlainText("<"+name+">");
        break;
    case 2:
        qDebug()<<"уведомление о новом участнике";
        out->setTextColor(*myName);
        out->insertPlainText("<"+name+">");
        break;
    case 3:

        break;
    case 20:
        qDebug()<<"обычное сообщение";
        if(name==tcpSocket->name)
            out->setTextColor(*myName);
        else
            out->setTextColor(*otherName);
        out->insertPlainText("<"+name+">");
        break;
    case 21:
        qDebug()<<"crypt message";
        break;
    }
    out->setTextColor(QColor(Qt::black));
    out->insertPlainText(mess+"\n");
    out->moveCursor(QTextCursor::End);
}

void MainWindow::getMess(int i, QString mess)
{
    int pos=mess.indexOf(">");
    QString nick,message;
    nick=mess.left(pos);
    message=mess.right(mess.length()-pos-1);
    messageToGui(i,nick,message);
    qDebug()<<nick<<message<<mess;

}

void MainWindow::connectSucces()
{
    ui->act_disconnect->setEnabled(true);
    ui->act_connect->setEnabled(false);
    ui->wd_out->setEnabled(true);
    ui->te_input->setEnabled(true);
    ui->statusBar->showMessage("Подключение успешно");
}

void MainWindow::inable()
{
    messageToGui(1,"system","Соединение потеряно!\n");
    ui->statusBar->showMessage("Разрыв соединения");
    this->on_act_disconnect_triggered();
}

void MainWindow::addUsersToGui(QString name)
{
    ui->lw_users->addItem(name);
}

void MainWindow::deleteUser(QString name)
{
    messageToGui(1,name,"покинул чат");
    for(int i=0;i<ui->lw_users->count();++i)
        if (ui->lw_users->item(i)->text()==name)
            ui->lw_users->takeItem(i);
}


void MainWindow::on_act_connect_triggered()
{
    win_connect *conn =new win_connect(this);
    connect(conn,SIGNAL(connectToServer(QString,QString,QString,QString)),
            this,SLOT(connectToServer(QString,QString,QString,QString)));
    conn->show();
}

void MainWindow::getText(QString text)
{
    emit sendMessage(text);
}

void MainWindow::on_act_disconnect_triggered()
{
    if(tcpSocket->getRuning())
    {
        ui->act_connect->setEnabled(true);
        tcpSocket->closeConnection();
        ui->act_disconnect->setEnabled(false);
        ui->statusBar->showMessage("Отключились от сервера");
        ui->te_input->setEnabled(false);
    }

}

void MainWindow::on_act_test_triggered()
{
    if (ui->act_test->isChecked())
        ui->mainToolBar->show();
    else
        ui->mainToolBar->hide();
}


void MainWindow::on_act_crypt_triggered()
{
    tcpSocket->_encrypt=!tcpSocket->_encrypt;
    qDebug()<<"encrypt"<<!tcpSocket->_encrypt;

}
