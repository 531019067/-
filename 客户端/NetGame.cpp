#include "NetGame.h"
#include <QMessageBox>
NetGame::NetGame(bool server, QHostAddress ip,QWidget *parent) : Board(parent)
{
    _server = NULL;
    _socket = NULL;
    _bServer = server;

    if(_bServer)
    {
        _server = new QTcpServer(this);
        _server->listen(QHostAddress::Any, 9899);
        connect(_server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    }
    else
    {
        _socket = new QTcpSocket(this);
	
        _socket->connectToHost(ip, 9899);
		if (_socket->waitForConnected(3000) == false)
		{
			QMessageBox::information(this,"tips","error");
		}
        connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));
    }
}

void NetGame::slotNewConnection()
{
    if(_socket) return;

    _socket = _server->nextPendingConnection(); //获取连接进来的socket
    connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));

    /* 产生随机数来决定谁走红色 */
    bool bRedSide = qrand()%2>0;
    init(bRedSide);

    /* 发送给对方 */
    QByteArray buf;
    buf.append(1);
    buf.append(bRedSide?0:1);
    _socket->write(buf);
}

void NetGame::back()
{
    if(_bRedTurn != _bSide)
        return;
    backOne();
    backOne();

    QByteArray buf;
    buf.append(3);
    _socket->write(buf);
}

void NetGame::click(int id, int row, int col)
{
    if(_bRedTurn != _bSide)
        return;

    Board::click(id, row, col);

    /* 发送给对方 */
    QByteArray buf;
    buf.append(2);
    buf.append(id);
    buf.append(row);
    buf.append(col);
    _socket->write(buf);
}

void NetGame::backFromNetwork(QByteArray)
{
    backOne();
    backOne();
}
void NetGame::clickFromNetwork(QByteArray buf)
{
    Board::click(buf[1], 9-buf[2], 8-buf[3]);
}
void NetGame::initFromNetwork(QByteArray buf)
{
    bool bRedSide = buf.at(1)>0?true:false;
    init(bRedSide);
}

void NetGame::slotDataArrive()
{
    QByteArray buf = _socket->readAll();
    switch (buf.at(0)) {
    case 1:
        initFromNetwork(buf);
        break;
    case 2:
        clickFromNetwork(buf);
        break;
    case 3:
        backFromNetwork(buf);
        break;
    default:
        break;
    }
}

NetGame::~NetGame()
{

}

