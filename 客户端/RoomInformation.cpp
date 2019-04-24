#include "RoomInformation.h"

RoomInformation::RoomInformation(int id,  QString ownerName, QString password , int playerNum , int observerNum)
	:_id(id), _ownerName(ownerName), _password(password),_playerNum(playerNum),_observerNum(observerNum),_ipAddress(QHostAddress("127.0.0.1"))
{
}

RoomInformation & RoomInformation::DeSerializable(const QByteArray & buf)
{
	//��ȡ�ļ�����Ϣ
	QByteArray tmp_buf = buf;
	QBuffer buffer(&tmp_buf);
	buffer.open(QIODevice::ReadWrite);

	QDataStream in(&buffer);
	in.setVersion(QDataStream::Qt_5_8);
	//ushort a1, a2;
	//�����л�, ��ȡ������Ϣ
	RoomInformation *roomInfo = new RoomInformation;
	//in >> a1 >> a2;
	in >> *roomInfo;

	buffer.close();
	return *roomInfo;
}

QByteArray & RoomInformation::Serializable(const RoomInformation & roomInfo)
{
	QByteArray *tmp_array = new QByteArray;
	QBuffer buffer(tmp_array);
	buffer.open(QIODevice::ReadWrite);

	QDataStream out(&buffer);
	out.setVersion(QDataStream::Qt_5_8);

	//���л�������Ϣ
	//�������������ռλ��, ���� ����type_id �� msg_len
	//out << ushort(0) << ushort(0) << userInfo;
	out << roomInfo;
	//out.device()->seek(0);
	//ushort len = (ushort)((*tmp_array).size());
	//out << (ushort)1 << len;

	buffer.close();
	return *tmp_array;
}

bool RoomInformation::operator==(const RoomInformation & rvalue)
{
	if (this->_id == rvalue._id)
		return true;
	else
		return false;
}


QDataStream & operator<<(QDataStream & out, const RoomInformation & roomInfo)
{
	out << roomInfo._id << roomInfo._ownerName << roomInfo._playerNum << roomInfo._observerNum << roomInfo._password<<roomInfo._ipAddress;
	return out;
}

QDataStream & operator>>(QDataStream & in, RoomInformation & roomInfo)
{
	in >> roomInfo._id >> roomInfo._ownerName >> roomInfo._playerNum >> roomInfo._observerNum >> roomInfo._password>>roomInfo._ipAddress;
	return in;
}
