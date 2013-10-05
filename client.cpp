#include <QTcpSocket>
#include <QDataStream>


#include "client.h"

client::client(QObject *parent) :
    QObject(parent)
{
    runing=false;
    ctx=new BLOWFISH_CTX;
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

void client::setCTX(BLOWFISH_CTX *ctx)
{
    client::ctx=ctx;
}

QString client::decrypt(QVector<int> vec)
{
    QString str;
    unsigned long L = 1, R = 2;
    QVector <int> vec2;
    vec2.resize(0);
    for(int i=0;i<(vec.size());i=i+2)
    {
        if(vec.size()-i>1)
        {
            L=vec.at(i);
            R=vec.at(i+1);
            Blowfish_Decrypt(ctx, &L, &R);
            vec2.push_back(L);
            vec2.push_back(R);
        }
        else
        {

            L=vec.at(i);
            R=vec2.at(0);
            Blowfish_Decrypt(ctx, &L, &R);
            vec2.push_back(L);
        }

    }

    for(int i=0;i<vec.size();++i)
        str+=QChar(vec2[i]);
    return str;
}

QVector<int> client::encrypt(QString str)
{
    QVector <int> vec;
    if (str!=""){
        vec.resize(0);
        for(int i=0;i<str.length();++i)
            vec.push_back(str.at(i).unicode());

        unsigned long L = 1, R = 2;
        for(int i=0;i<(vec.size());i=i+2)
        {

            if(vec.size()-i>1)
            {
                L=vec.at(i);
                R=vec.at(i+1);
                Blowfish_Encrypt(ctx, &L, &R);
                vec[i]=(L);
                vec[i+1]=(R);
            }
            else
            {
                L=vec.at(i);
                R=32;
                Blowfish_Encrypt(ctx, &L, &R);
                vec[i]=(L);
                vec.push_back(R);
            }
        }
    }
    return vec;
}

void client::inabled()
{
    emit disconnected();
    runing=false;

}

void client::sendToServerMessage(QString mess)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    if(!_encrypt){
        out << (quint16)0 << (quint8)20 << mess;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        _sok->write(block);
        qDebug()<<"отправлено"<<20<<" "<<mess<<"пользователю";
    }
    else
    {
        QVector <int> vec=encrypt(mess);
        out << (quint16)0 << (quint8)21 <<vec;
        qDebug()<<21<<vec;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        _sok->write(block);
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
        case 21:
        {
            QVector <int> vec;
            vec.resize(0);
            in>>temp;
            qDebug()<<temp;
            in>>vec;
            qDebug()<<vec;
            temp+=decrypt(vec);
            emit getMessage(20,temp);

        }
            break;

        }
        qDebug()<<"---------------------";
        _blockSize = 0;
    }
}
