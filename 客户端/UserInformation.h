#pragma once
#include <QString>
#include <QDataStream>
#include <QBuffer>
#include <QByteArray>
class UserInformation
{
public:
	int _id;
	QString _username;
	QString _account;
	QString _password;
	int _rank;
	int _score;
	int _gold;
public:
	UserInformation(int id,QString usr,QString acc,QString pwd,int rank,int score,int gold);
	UserInformation();
	~UserInformation();
	QString GetAccount();
	QString GetPassword();
	friend QDataStream& operator<<(QDataStream& out, const UserInformation& userInfo);
	friend QDataStream& operator>>(QDataStream& in, UserInformation& userInfo);
	// 反序列化和序列化
	static UserInformation& DeSerializable(const QByteArray &buf);
	static QByteArray& Serializable(const UserInformation &userInfo);
};
