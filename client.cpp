#include <QTcpSocket>
#include <QDataStream>
#include <QTcpServer>
#include <QUdpSocket>

#include "client.h"


client::client(QObject *parent) :
    QObject(parent)
{
    _blockSize=0;
    runing=false;
//    ctx=new BLOWFISH_CTX;
    _sok=new QTcpSocket(this);
    connect(_sok,SIGNAL(connected()),this,SLOT(connectSucces()));
    connect(_sok,SIGNAL(disconnected()),this,SLOT(inabled()));
    connect(_sok,SIGNAL(readyRead()),this,SLOT(readServ()));
    connect(_sok,SIGNAL(error(QAbstractSocket::SocketError)),this,SIGNAL(displayError(QAbstractSocket::SocketError)));
    QUdpSocket *udp=new QUdpSocket(this);
    udp->bind(QHostAddress::Any,(quint16)1024);

}

void client::connectTo(QString addr, quint16 port)
{
    _sok->connectToHost(addr,port);

}

void client::closeConnection()
{
    _sok->close();
}

void client::send(quint8 cmd, QString mess)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    if(!_encrypt){
        out << (quint16)0 << (quint8)cmd << mess;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        _sok->write(block);
    }
}

void client::setCTX(BLOWFISH_CTX *ctx)
{
    client::ctx=ctx;
}

void client::call(QString name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << (quint8)30<<name;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
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
    emit connected();
}

client::~client()
{
// delete ctx;
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
        case 1:
            in>>temp;
            emit getMessage(1,"system>"+temp);
            break;
        case 31:
        {

            break;
        }
        case 32:
        {

            break;
        }
        case 25:
        {
            in>>temp;
            qDebug()<<temp;

            //_sok->setPeerPort(1025);
            QTcpServer *serv= new QTcpServer(this);


            if (!serv->listen(QHostAddress::Any,(quint16)temp.toInt()))
            {
                qDebug() << "Server not started at" ;

            }
            break;
        }

        case 40:
        {
            QStringList lstRoom;
            in>>lstRoom;
            emit displayListRoom(lstRoom);
            break;
        }


        }
        qDebug()<<"---------------------";
        _blockSize = 0;
    }
}
