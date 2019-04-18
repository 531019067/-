#pragma once
#include "LoginGUI.h"
#include "UserInformation.h"
#include "RoomInformation.h"
#include "RoomWnd.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>


class Lobby :public QWidget
{
	Q_OBJECT
public:
	explicit Lobby(QWidget *parent = 0);
	~Lobby();
	
	void init();
	bool login();
	void createBattle(QByteArray data);
	void joinBattle(QByteArray data);
	void loadRoomList(QByteArray data);

signals:
	void loginSuccess();

public slots:
	void slotCreateBattle();
	void slotJoinBattle();
	void slotDataArrive();
	void slotLoadRoomListWidget();
private:
	UserInformation _userInfo;
	QTcpSocket *_socket;
	RoomWnd * _roomWnd;
	QByteArray  _totalBuffer;

};
