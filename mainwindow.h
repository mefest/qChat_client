#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "blowfish.h"

class QTcpSocket;
class client;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    BLOWFISH_CTX *ctx;
    bool encrypt;

private slots:
    void connectToServer(QString name,QString addrServ,QString portServ, QString key);
    void messageToGui(int kod, QString name, QString mess);
    void getMess(int i, QString mess);
    void connectSucces();
    void inable();
    void addUsersToGui(QString name);
    void deleteUser(QString name);
    void on_act_connect_triggered();


    void on_act_disconnect_triggered();

    void on_act_test_triggered();

signals:
    void sendMessage(QString mess);

protected:

  virtual void 	keyPressEvent ( QKeyEvent * event );


private:
    QColor *myName;
    QColor *otherName;
    client    *tcpSocket;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
