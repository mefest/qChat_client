#include <QTcpSocket>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "win_connect.h"
#include "client.h"
#include "settings.h"
#include "saveload.h"

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
    font= new QFont();
    saveLoad *load= new saveLoad(this);
    load->load(myName,otherName,font);
    delete load;
    ui->te_message->setFont(*font);
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
    connect(tcpSocket,SIGNAL(displayError(QAbstractSocket::SocketError)),this,SLOT(socError(QAbstractSocket::SocketError)));

    createMenuUserList();

    //<--here

#ifdef Q_OS_WIN32
    tray = new QSystemTrayIcon(QIcon(":/icon/resource/icon/icon.ico"),this);
    tray->show();
    QMenu *Menu = new QMenu(this);
    QAction *showw = new QAction("Show", this);
    connect(showw, SIGNAL(triggered()), this, SLOT(showEvent()));
    QAction *quitAction = new QAction("Quit", this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(closeEvent2()));
    Menu->addAction(showw);
    Menu->addAction(quitAction);
    tray->setContextMenu(Menu);

#endif

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenuUserList()
{
    menuUsers = new QMenu(this);
    menuUsers->addAction(ui->act_Call);
}

void MainWindow::connectToServer(QString name, QString addrServ, QString portServ, QString key)
{
    if(!tcpSocket->getRuning())
    {
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
        tcpSocket->_encrypt=false;
        ui->act_crypt->setChecked(false);
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
        out->insertPlainText("sdfs");
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
    ui->lw_users->clear();
    this->on_act_disconnect_triggered();
}

void MainWindow::addUsersToGui(QString name)
{
    QListWidgetItem *newUser = new QListWidgetItem(QIcon(":/icon/resource/icon/user_black.ico"),name);
    ui->lw_users->addItem(newUser);
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
    if(text!="")
    {
        if(text.length()>256)
            text=text.mid(0,255);
        emit sendMessage(text);
    }
}

void MainWindow::socError(QAbstractSocket::SocketError socketError)
{

    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("QChat Client"),
                                 tr("Хост не найден. Пожалуйста проверьте "
                                    "настройки сервера."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("QChat Client"),
                                 tr("The connection was refused by the peer. "
                                    "Убедитесь, что сервер запущен "
                                    "и провельте адресс сервера и порт. "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("QChat Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->_sok->errorString()));
    }

}

void MainWindow::closeEvent2()
{
    closeid=2;
    this->close();
}

void MainWindow::showEvent()
{
    this->show();
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

    tcpSocket->_encrypt=ui->act_crypt->isChecked();
    qDebug()<<"encrypt"<<tcpSocket->_encrypt;

}

void MainWindow::on_act_getIp_triggered()
{
    // tcpSocket->send(25,"ip");

}

void MainWindow::on_actionPeerPOrt_triggered()
{
    // tcpSocket->_sok->setPeerPort((quint16)ui->te_input->toPlainText().toInt());
}

void MainWindow::on_act_setting_triggered()
{
    settings *set = new settings(this);
    //обрати внимание
    set->exec();
    saveLoad *load= new saveLoad(this);
    load->load(myName,otherName,font);
    delete load;
    ui->te_message->setFont(*font);

}

void MainWindow::closeEvent(QCloseEvent *eClose)
{
#ifdef Q_OS_WIN32
    if(closeid!=2){
        this->hide();
        eClose->ignore();
    } else {
        _serv->stopServer();
        eClose->accept();
    }
#endif
}

void MainWindow::on_act_About_triggered()
{
    QMessageBox::information(this,"About","qChat\n"

                             "Над прогораммой работали:\n"
                             "Фролов Максим.\n"
                             "Федоров Денис (иконка приложения).\n"

                             "Если вы найдете ошибку или захотите связаться с автором - fral_@mail.ru");
}

void MainWindow::on_lw_users_customContextMenuRequested(const QPoint &pos)
{
    menuUsers->exec(ui->lw_users->mapToGlobal(pos));
}

void MainWindow::on_act_Call_triggered()
{
    if(ui->lw_users->count()>0)
        tcpSocket->call(ui->lw_users->selectedItems().at(0)->text());
}
