#pragma once
#include "LoginGUI.h"
#include "UserInformation.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>

class Lobby :public QMainWindow
{
	Q_OBJECT
public:
	explicit Lobby(QWidget *parent = 0);
	~Lobby();
	UserInformation _userInfo;
	QLabel *_createBattle;
	QLabel *_joinBattle;
	QTcpSocket *_socket;

	void init();
	bool login();

public slots:

	//void slotDataArrive();

};
