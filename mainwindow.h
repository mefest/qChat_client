#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "blowfish.h"
#include "messinput.h"
#include <QAbstractSocket>

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
    void createMenuUserList();

private slots:
    void connectToServer(QString name,QString addrServ,QString portServ, QString key);
    void messageToGui(int kod, QString name, QString mess);
    void getMess(int i, QString mess);
    void connectSucces();
    void inable();
    void addUsersToGui(QString name);
    void deleteUser(QString name);
    void on_act_connect_triggered();
    void getText(QString text);
    void socError(QAbstractSocket::SocketError socketError);
    void closeEvent2();
    void showEvent();
    void displayRooms(QStringList);

    void on_act_disconnect_triggered();

    void on_act_test_triggered();

    void on_act_crypt_triggered();

    void on_act_getIp_triggered();

    void on_actionPeerPOrt_triggered();

    void on_act_setting_triggered();

    void on_act_About_triggered();


    void on_lw_users_customContextMenuRequested(const QPoint &pos);

    void on_act_Call_triggered();

signals:
    void sendMessage(QString mess);

protected:


private:
    QColor *myName;
    QColor *otherName;
    QFont *font;
    client    *tcpSocket;
    QMenu *menuUsers;
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *eClose);
    int closeid;
};

#endif // MAINWINDOW_H
