#include "RoomWnd.h"
#include <QScrollArea>
#pragma execution_character_set("utf-8")
RoomWnd::RoomWnd(QWidget *parent/* =0 */)
	: QWidget(parent),
	_count(0),
	_vLayout(new QVBoxLayout(this)),
	_roomList(new QListWidget(this))
{
	resize(400, 400);
	QLabel *rowText = new QLabel(this);
	_vLayout->addWidget(_roomList);
	_vLayout->addWidget(rowText);
	connect(_roomList, SIGNAL(currentTextChanged(const QString &)), rowText, SLOT(setText(const QString &)));

}
RoomWnd::~RoomWnd()
{
}

void RoomWnd::addRoom(const RoomInformation & room)
{
	
	QString str = "房间号：" + QString::number(room._id) + ' ' + room._ownerName + "的房间" + "       人数 1/2";
	QListWidgetItem* temp = new QListWidgetItem(QIcon(".\\Resources\\BA"), str,_roomList);
	_roomList->insertItem(_roomList->count(), temp);

	update();
}

void RoomWnd::removeRoom(int id)
{

}
