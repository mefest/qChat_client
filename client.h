#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "blowfish.h"
#include <QAbstractSocket>

#include "voip.h"


class QTcpSocket;

class client : public QObject
{
    Q_OBJECT
public:
    QString name;
    bool _encrypt;
    BLOWFISH_CTX *ctx;
    explicit client(QObject *parent = 0);
    bool getRuning() const {return runing;}
    void connectTo(QString addr, quint16 port);
    void closeConnection();
    void send(quint8 cmd,QString mess);
    void setCTX(BLOWFISH_CTX *ctx);
    void call(QString name);
    QString decrypt(QVector <int> vec);
    QVector<int> encrypt(QString str);
    QTcpSocket *_sok;
    voip *voipClient;


signals:
    void delUser(QString name);
    void connected();
    void disconnected();
    void addUsers(QString name);
    void getMessage(int i,QString mess);
    void displayError(QAbstractSocket::SocketError socketError);

public slots:
    void inabled();
    void sendToServerMessage(QString mess);
    void connectSucces();
    void readServ();

private:
    bool runing;
    quint16 _blockSize=0;




};

#endif // CLIENT_H
