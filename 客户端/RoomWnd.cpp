#include "RoomWnd.h"
#include <QScrollArea>
#pragma execution_character_set("utf-8")
RoomWnd::RoomWnd(QWidget *parent/* =0 */)
	: QWidget(parent),
	_count(0),
	_vLayout(new QVBoxLayout(this)),
	_roomListWidget(new QListWidget(this)),
	_udpSocket(new QUdpSocket(this))
{
	resize(400, 400);
	QLabel *rowText = new QLabel(this);
	_vLayout->addWidget(_roomListWidget);
	_vLayout->addWidget(rowText);
	connect(_roomListWidget, SIGNAL(currentTextChanged(const QString &)), rowText, SLOT(setText(const QString &)));
	if (!_udpSocket->bind(UDP_PORT))
	{
		qDebug() << "udp socket bind error";
		QMessageBox::information(this, "tips", "bind port error");
	}
	connect(_udpSocket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));

}
RoomWnd::~RoomWnd()
{
}



void RoomWnd::addRoom(const RoomInformation & room)
{
	int row = 0;
	while (_roomIdList.size() > row&&room._id > _roomIdList.at(row))
		row++;
	QString str = "房间号：" + QString::number(room._id) + ' ' + room._ownerName + "的房间" + "       人数"+QString::number(room._playerNum)+"/2";
	QListWidgetItem* temp = new QListWidgetItem(str);
	_roomListWidget->insertItem(row,temp);
	_roomIdList.insert(row,room._id);
	update();
}

void RoomWnd::removeRoom(const RoomInformation &room)
{

	int row=_roomIdList.indexOf(room._id);

	QListWidgetItem * p = this->_roomListWidget->takeItem(row);
	delete p;
	QList<int>::iterator it = _roomIdList.begin();
	for (int j = 0; j < row; ++j)		
		++it;
	_roomIdList.erase(it);
	update();
}
void RoomWnd::updateRoomList(const RoomInformation&room)
{
	int row = _roomIdList.indexOf(room._id);
	QString str = "房间号：" + QString::number(room._id) + ' ' + room._ownerName + "的房间" + "       人数" + QString::number(room._playerNum) + "/2";
	QListWidgetItem * p = this->_roomListWidget->takeItem(row);
	p->setText(str);
	_roomListWidget->insertItem(row, p);
	update();
	
}

void RoomWnd::slotDataArrive() 
{
	while (_udpSocket->hasPendingDatagrams())//判断udpsocket里面是否有数据报可读
	{
		QByteArray datagram;
		datagram.resize(_udpSocket->pendingDatagramSize());//获得第一个数据报长度
		_udpSocket->readDatagram(datagram.data(), datagram.size());//读取第一个数据报信息
		QDataStream in(datagram);
		ushort typeId;
		in >> typeId;
		RoomInformation room;
		in >> room;
		switch (typeId)
		{
		case 1:
			addRoom(room);
			break;
		case 2:
			removeRoom(room);
			break;
		case 3:
			updateRoomList(room);
			break;
		}
		
	}
}