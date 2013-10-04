#include <QTcpSocket>
#include <QDataStream>


#include "client.h"

client::client(QObject *parent) :
    QObject(parent)
{
    runing=false;

    _sok=new QTcpSocket(this);
    connect(_sok,SIGNAL(connected()),this,SLOT(connectSucces()));
    connect(_sok,SIGNAL(disconnected()),this,SLOT(inabled()));
    connect(_sok,SIGNAL(readyRead()),this,SLOT(readServ()));

}

void client::connectTo(QString addr, quint16 port)
{
    _sok->connectToHost(addr,port);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << (quint8)0 << (QString)"AuthReq";
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
    block.clear();
    out.device()->seek(0);
    out << (quint16)0 << (quint8)1 <<name;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}

void client::closeConnection()
{
    _sok->close();
}

void client::inabled()
{
    emit disconnected();
    runing=false;

}

void client::sendToServerMessage(QString mess)
{
    if(!encrypt){


        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << (quint8)20 << mess;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        _sok->write(block);
        qDebug()<<"отправлено"<<20<<" "<<mess<<"пользователю";
    }

}

void client::connectSucces()
{
    runing=true;
    emit connected();
}

void client::readServ()
{
    QDataStream in(_sok);
    QString temp;
    while(!in.atEnd())
    {
        if(_blockSize==0){
            if (_sok->bytesAvailable() < (int)sizeof(quint16))
                return;
            in >> _blockSize;
            qDebug()<<"Полученно------------";
            qDebug()<<"размер"<<_blockSize;
        }
        if (_sok->bytesAvailable() < _blockSize)
            return;
        else
            _blockSize = 0;
        quint8 command;
        in >> command;
        qDebug()<<"команда"<<command;
        switch (command)
        {
        case 20:
        {
            in>>temp;
            QString mess;
            in>>mess;
            qDebug()<<temp<<mess;
            emit getMessage(20,temp+">"+mess);

            break;
        }
        case 0:
            in>>temp;
            qDebug()<<temp;
            break;
        case 12:
        {
            in>>temp;
            qDebug()<<temp;
            emit addUsers(temp);
            break;
        }
        case 13:
            in>>temp;
            emit delUser(temp);
            qDebug()<<"del"<<temp;
            break;
        case 2:
            in >>temp;
            emit getMessage(2,temp);
            break;

        }
        qDebug()<<"---------------------";
        _blockSize = 0;
    }
}
