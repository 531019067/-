#ifndef NETGAME_H
#define NETGAME_H

#include "Board.h"
#include <QTcpServer>
#include <QTcpSocket>



class NetGame : public Board
{
    Q_OBJECT
public:
    explicit NetGame(bool server,QHostAddress ip= QHostAddress("127.0.0.1"), QWidget *parent = 0);
    ~NetGame();

    bool _bServer;
    QTcpServer* _server;
    QTcpSocket* _socket;

    void back();
    void click(int id, int row, int col);

    void backFromNetwork(QByteArray buf);
    void clickFromNetwork(QByteArray buf);
    void initFromNetwork(QByteArray buf);

signals:

public slots:
    void slotNewConnection();
    void slotDataArrive();
};

#endif // NETGAME_H
