#pragma once
#include <QWidget>
#include <QList>
#include <QLabel>
#include <QUdpSocket>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <QListWidget>
#include <QListWidgetItem>
#include "RoomInformation.h"
class RoomWnd : public QWidget
{
	Q_OBJECT
public:
	explicit RoomWnd(QWidget *parent = 0);
	~RoomWnd();
public slots:
	void slotDataArrive();
public:
	const int UDP_PORT = 9897;
	QListWidget *_roomListWidget;
	QVBoxLayout * _vLayout;
	QTcpSocket *_tcpSocket;
	QUdpSocket *_udpSocket;
	int _count;
	QList<int> _roomIdList;
	void updateRoomList(const RoomInformation&room);
	void addRoom(const RoomInformation &room);
	void removeRoom(const RoomInformation &room);

};