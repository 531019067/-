#pragma once
#include "UserInformation.h"
class RoomInformation
{
public:
	RoomInformation(int id = 0, QString ownerName = "", QString password = "",int playerNum=1,int observerNum=0);
	friend QDataStream& operator<<(QDataStream& out, const RoomInformation& roomInfo);
	friend QDataStream& operator>>(QDataStream& in, RoomInformation& roomInfo);
	// 反序列化和序列化
	static RoomInformation& DeSerializable(const QByteArray &buf);
	static QByteArray& Serializable(const RoomInformation &roomInfo);
public:
	int _id;//房间号
	int _playerNum;//玩家数(1或者2)
	int _observerNum;//观战数
	QString _ownerName;//房间拥有者名字
	QString _password;//房间密码

};
