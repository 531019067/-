#include "UserInformation.h"

UserInformation::UserInformation(int id, QString usr, QString acc, QString pwd, int rank, int score, int gold)
	:_id(id),_username(usr),_account(acc),_password(pwd),_rank(rank),_score(score),_gold(gold)
{

}

UserInformation::UserInformation()
{
}

UserInformation::~UserInformation()
{
}

QString UserInformation::GetAccount()
{
	return this->_account;
}

QString UserInformation::GetPassword()
{
	return _password;
}

QDataStream & operator<<(QDataStream & out, const UserInformation & userInfo)
{
	out << userInfo._id << userInfo._username << userInfo._account << userInfo._password << userInfo._rank << userInfo._score << userInfo._gold;
	return out;
	// TODO: 在此处插入 return 语句
}

QDataStream & operator>>(QDataStream & in, UserInformation & userInfo)
{
	// TODO: 在此处插入 return 语句
	in >> userInfo._id >> userInfo._username >> userInfo._account >> userInfo._password >> userInfo._rank >> userInfo._score >> userInfo._gold;
	return in;
}
UserInformation & UserInformation::DeSerializable(const QByteArray & buf)
{
	//读取文件流信息
	QByteArray tmp_buf = buf;
	QBuffer buffer(&tmp_buf);
	buffer.open(QIODevice::ReadWrite);

	QDataStream in(&buffer);
	in.setVersion(QDataStream::Qt_5_8);
	//ushort a1, a2;
	//反序列化, 获取对象信息
	UserInformation *userInfo=new UserInformation;
	//in >> a1 >> a2;
	in >> *userInfo;

	buffer.close();
	return *userInfo;
}

QByteArray & UserInformation::Serializable(const UserInformation & userInfo)
{
	QByteArray *tmp_array = new QByteArray;
	QBuffer buffer(tmp_array);
	buffer.open(QIODevice::ReadWrite);

	QDataStream out(&buffer);
	out.setVersion(QDataStream::Qt_5_8);

	//序列化对象信息
	//在这里插入两个占位符, 用于 放置type_id 和 msg_len
	//out << ushort(0) << ushort(0) << userInfo;
	out << userInfo;
	//out.device()->seek(0);
	//ushort len = (ushort)((*tmp_array).size());
	//out << (ushort)1 << len;

	buffer.close();
	return *tmp_array;
}