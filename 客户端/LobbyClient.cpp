#include "LobbyClient.h"
#include "CtrlBattlePanel.h"
#include "MainWnd.h"
#pragma execution_character_set("utf-8")
Lobby::Lobby(QWidget *parent /* = 0 */):QWidget(parent)
{
	this->setWindowTitle("游戏大厅");
	setMinimumSize(500,500);
	_roomWnd = new RoomWnd(this);
	QPixmap pixmap = QPixmap(".\\Resources\\background.jpg").scaled(this->size());
	QPalette  palette(this->palette());
	palette.setBrush(QPalette::Background, QBrush(pixmap));
	this->setPalette(palette);

	_socket = new QTcpSocket(this);
	_socket->connectToHost(QHostAddress("127.0.0.1"), 9898);
	if (_socket->waitForConnected(3000) == false)
		QMessageBox::information(this, "tips", "服务器无法连接");
	connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));
	connect(this, SIGNAL(loginSuccess()), this, SLOT(slotLoadRoomListWidget()));
}
Lobby::~Lobby()
{

}

bool Lobby::login()
{
	LoginGUI *login = new LoginGUI(_socket);
	disconnect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));
	if (login->exec() != QDialog::Accepted)
		return false;
	
	_userInfo = login->getUserInfo();
	emit loginSuccess();
	login->disconnect(_socket, SIGNAL(readyRead()), login, SLOT(slotDataArrive()));
	connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));
	login->close();
	delete login;
	return true;
}
void Lobby::createBattle(QByteArray data)
{
	QDataStream in(data);
	RoomInformation *room=new RoomInformation;
	in >> *room;

	_chessWnd = new MainWnd(2,room);
	connect(_chessWnd, SIGNAL(quitBattle()), this, SLOT(slotQuitBattle()));
	_chessWnd->show();


}
void Lobby::joinBattle(QByteArray data)
{
	QDataStream in(data);
	RoomInformation *room=new RoomInformation();
	in >> *room;
	if (room->_playerNum==1)
	{
		_chessWnd = new MainWnd(3,room);
		connect(_chessWnd, SIGNAL(quitBattle()), this, SLOT(slotQuitBattle()));
		_chessWnd->show();
		
	}
	else if(room->_playerNum== 2)
	{
		QMessageBox::information(this, "提醒", "房间已满");
		
	}
}
void Lobby::loadRoomList(QByteArray data)
{
	QDataStream in(data);
	int len;
	in >> len;
	for (int i = 0; i < len; i++)
	{
		RoomInformation room;
		in >> room;
		_roomWnd->addRoom(room);
	}
	
}




void Lobby::slotCreateBattle()
{
	QByteArray data;
	data.append(3);
	data.append(' ');
	data.append(_userInfo._username);
	_socket->write(data);
	

}
void Lobby::slotJoinBattle()
{
	QByteArray data;
	int row = _roomWnd->_roomListWidget->currentRow();
	data.append(4);
	data.append(' ');
	data.append(row);
	_socket->write(data);
}
void Lobby::slotQuitBattle()
{
	if (_chessWnd == nullptr)
		return;
	QByteArray data;
	data.append(6);
	if (_chessWnd->_gameType == 2)//服务器
		data.append(2);
	else if (_chessWnd->_gameType == 3)//客户端
		data.append(3);
	else
		return;

	data.append(_chessWnd->_currentRoom->_id);
	_socket->write(data);
}

void Lobby::slotLoadRoomListWidget()
{
	QByteArray data;
	data.append(5);
	_socket->write(data);
}
void Lobby::slotExitBattle()
{
	QByteArray data;
	QMessageBox::information(this, "tips", "quit");
}
void Lobby::slotDataArrive()
{
	//缓冲区没有数据，直接无视
	if (_socket->bytesAvailable() <= 0)
	{
		return;
	}
	//临时获得从缓存区取出来的数据
	QByteArray tempBuffer = _socket->readAll();
	//上次缓存加上这次数据
	_totalBuffer.append(tempBuffer);
	ushort typeId, msgLen;
	int totalLen = _totalBuffer.size();//当前缓存的长度
	while (totalLen)
	{
		//与QDataStream绑定，方便操作。
		QDataStream packet(_totalBuffer);
		//设置为大端模式
		packet.setByteOrder(QDataStream::BigEndian);

		packet >> typeId >> msgLen;
		//如果不够长度等够了在来解析
		if (totalLen < msgLen)
		{
			break;
		}
		switch (typeId)
		{
		case 3:
			createBattle(_totalBuffer.right(msgLen-4));
			break;
		case 4:
			joinBattle(_totalBuffer.right(msgLen - 4));
			break;
		case 5:
			loadRoomList(_totalBuffer.right(msgLen - 4));
		default:
			break;
		}
		//缓存多余的数据
		tempBuffer = _totalBuffer.right(totalLen - msgLen);
		//更新长度
		totalLen = tempBuffer.size();
		//更新多余数据
		_totalBuffer = tempBuffer;
	}

}

void Lobby::init()
{	
	
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	CtrlBattlePanel * bottomPanel = new CtrlBattlePanel();
	mainLayout->addWidget(_roomWnd);
	mainLayout->addWidget(bottomPanel, 1,Qt::AlignBottom);
	connect(bottomPanel, SIGNAL(sigCreate()), this, SLOT(slotCreateBattle()));
	connect(bottomPanel, SIGNAL(sigJoin()), this, SLOT(slotJoinBattle()));
	

}

