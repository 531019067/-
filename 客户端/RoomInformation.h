#pragma once
#include "UserInformation.h"
class RoomInformation
{
public:
	RoomInformation(int id = 0, QString ownerName = "", QString password = "",int playerNum=1,int observerNum=0);
	friend QDataStream& operator<<(QDataStream& out, const RoomInformation& roomInfo);
	friend QDataStream& operator>>(QDataStream& in, RoomInformation& roomInfo);
	// �����л������л�
	static RoomInformation& DeSerializable(const QByteArray &buf);
	static QByteArray& Serializable(const RoomInformation &roomInfo);
public:
	int _id;//�����
	int _playerNum;//�����(1����2)
	int _observerNum;//��ս��
	QString _ownerName;//����ӵ��������
	QString _password;//��������

};
