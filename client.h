#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

class QTcpSocket;

class client : public QObject
{
    Q_OBJECT
public:
    QString name;
    bool encrypt;
    explicit client(QObject *parent = 0);
    bool getRuning() const {return runing;}
    void connectTo(QString addr, quint16 port);
    void closeConnection();
    void send();

signals:
    void delUser(QString name);
    void connected();
    void disconnected();
    void addUsers(QString name);
    void getMessage(int i,QString mess);

public slots:
    void inabled();
    void sendToServerMessage(QString mess);
    void connectSucces();
    void readServ();

private:
    bool runing;
    quint16 _blockSize;


    QTcpSocket *_sok;

};

#endif // CLIENT_H
