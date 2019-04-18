#pragma once
#include <QWidget>
#include <QList>
#include <QLabel>

#include <QPushButton>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <QListWidget>
#include <QListWidgetItem>
#include "RoomInformation.h"
class RoomWnd : public QWidget
{
	Q_OBJECT
public:
	explicit RoomWnd(QWidget *parent=0);
	~RoomWnd();
public:
	QListWidget *_roomList;
	QVBoxLayout * _vLayout;
	QTcpSocket *_socket;
	int _count;
	//QList<RoomInformation> *_roomList;

	void addRoom(const RoomInformation &room);
	void removeRoom(int id);

};