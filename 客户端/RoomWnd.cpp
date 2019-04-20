#include "RoomWnd.h"
#include <QScrollArea>
#pragma execution_character_set("utf-8")
RoomWnd::RoomWnd(QWidget *parent/* =0 */)
	: QWidget(parent),
	_count(0),
	_vLayout(new QVBoxLayout(this)),
	_roomList(new QListWidget(this)),
	_udpSocket(new QUdpSocket(this))
{
	resize(400, 400);
	QLabel *rowText = new QLabel(this);
	_vLayout->addWidget(_roomList);
	_vLayout->addWidget(rowText);
	connect(_roomList, SIGNAL(currentTextChanged(const QString &)), rowText, SLOT(setText(const QString &)));
	if (_udpSocket->bind(UDP_PORT))
	{
		qDebug() << "udp socket bind error";
	}
	connect(_udpSocket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));

}
RoomWnd::~RoomWnd()
{
}



void RoomWnd::addRoom(const RoomInformation & room)
{
	
	QString str = "����ţ�" + QString::number(room._id) + ' ' + room._ownerName + "�ķ���" + "       ����"+QString::number(room._playerNum)+"/2";
	QListWidgetItem* temp = new QListWidgetItem(QIcon(".\\Resources\\BA"), str,_roomList);
	_roomList->insertItem(room._id-1, temp);

	update();
}

void RoomWnd::removeRoom(int id)
{
	QListWidgetItem * p = this->_roomList->takeItem(id);
	delete p;
}
void RoomWnd::updateRoomList(const RoomInformation&room)
{
	removeRoom(room._id);
	addRoom(room);
	
}

void RoomWnd::slotDataArrive() 
{
	while (_udpSocket->hasPendingDatagrams())//�ж�udpsocket�����Ƿ������ݱ��ɶ�
	{
		QByteArray datagram;
		datagram.resize(_udpSocket->pendingDatagramSize());//��õ�һ�����ݱ�����
		_udpSocket->readDatagram(datagram.data(), datagram.size());//��ȡ��һ�����ݱ���Ϣ
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
			removeRoom(room._id);
			break;
		case 3:
			updateRoomList(room);
			break;
		}
		
	}
}