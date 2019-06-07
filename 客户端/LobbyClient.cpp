#include "LobbyClient.h"
#include "CtrlBattlePanel.h"
#include "MainWnd.h"
#pragma execution_character_set("utf-8")
Lobby::Lobby(QWidget *parent /* = 0 */):QWidget(parent)
{
	this->setWindowTitle("��Ϸ����");
	setMinimumSize(500,500);
	_roomWnd = new RoomWnd(this);
	QPixmap pixmap = QPixmap(".\\Resources\\background.jpg").scaled(this->size());
	QPalette  palette(this->palette());
	palette.setBrush(QPalette::Background, QBrush(pixmap));
	this->setPalette(palette);

	_socket = new QTcpSocket(this);
	_socket->connectToHost(QHostAddress("127.0.0.1"), 9898);
	if (_socket->waitForConnected(3000) == false)
		QMessageBox::information(this, "tips", "�������޷�����");
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
		QMessageBox::information(this, "����", "��������");
		
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
	if (_chessWnd->_gameType == 2)//������
		data.append(2);
	else if (_chessWnd->_gameType == 3)//�ͻ���
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
	//������û�����ݣ�ֱ������
	if (_socket->bytesAvailable() <= 0)
	{
		return;
	}
	//��ʱ��ôӻ�����ȡ����������
	QByteArray tempBuffer = _socket->readAll();
	//�ϴλ�������������
	_totalBuffer.append(tempBuffer);
	ushort typeId, msgLen;
	int totalLen = _totalBuffer.size();//��ǰ����ĳ���
	while (totalLen)
	{
		//��QDataStream�󶨣����������
		QDataStream packet(_totalBuffer);
		//����Ϊ���ģʽ
		packet.setByteOrder(QDataStream::BigEndian);

		packet >> typeId >> msgLen;
		//����������ȵȹ�����������
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
		//������������
		tempBuffer = _totalBuffer.right(totalLen - msgLen);
		//���³���
		totalLen = tempBuffer.size();
		//���¶�������
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

