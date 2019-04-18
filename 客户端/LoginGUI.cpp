#include "LoginGUI.h"
#pragma execution_character_set("utf-8")
LoginGUI::LoginGUI(QTcpSocket *socket,QWidget *parent) :_socket(socket),QDialog(parent),_tempAccount("."),_tempPassword(".")
{

	_userInfo = new UserInformation;
	_headPixmap = new QLabel();
	_accLabel = new QLabel("账  号");
	_pwdLabel = new QLabel("密  码");
	_accText = new QLineEdit();
	_pwdText = new QLineEdit();
	_login = new QPushButton("登录");
	_signUp = new QPushButton("注册账号");
	_retrieve = new QPushButton("找回密码");
	_rememberBox = new QCheckBox("记住密码");
	_autoLoginBox = new QCheckBox("自动登录");
	init();
	

}


LoginGUI::~LoginGUI()
{


}
UserInformation LoginGUI::getUserInfo()
{
	return *_userInfo;
}



void LoginGUI::init()
{
	QGridLayout *mainLayout = new QGridLayout(this);
	//头像
	QPixmap headPicture(".\\Resources\\RJIANG");
	
	_headPixmap->setFixedSize(90, 90);
	_headPixmap->setPixmap(headPicture);
	_headPixmap->setScaledContents(true);
	//文本输入
	
	_accText->setPlaceholderText("请输入你的用户名");
	_accText->setFocus();
	
	_pwdText->setPlaceholderText("请输入你的密码");
	_pwdText->setEchoMode(QLineEdit::Password);
	//按钮
	
	_login->setDefault(true);
	connect(_login, SIGNAL(clicked()), this, SLOT(slotSignIn()));
	
	connect(_signUp, SIGNAL(clicked()), this, SLOT(slotSignUp()));
	

	mainLayout->addWidget(_headPixmap, 0, 0, 2, 1);//头像
	//第一行
	mainLayout->addWidget(_accLabel, 0, 1, 1, 1);
	mainLayout->addWidget(_accText, 0, 2, 1, 2);
	mainLayout->addWidget(_signUp, 0, 4);//注册账号按钮
	//第二行
	mainLayout->addWidget(_pwdLabel, 1, 1, 1, 1);
	mainLayout->addWidget(_pwdText, 1, 2, 1, 2);
	mainLayout->addWidget(_retrieve, 1, 4);//找回密码按钮
	//第三行
	mainLayout->addWidget(_rememberBox, 2, 1, 1, 1, Qt::AlignRight | Qt::AlignVCenter);//记住密码 第2行，第1列开始，占1行1列 水平居左 垂直居中
	mainLayout->addWidget(_autoLoginBox, 2, 2, 1, 1, Qt::AlignRight | Qt::AlignVCenter);//自动登录 第2行，第2列开始，占1行1列 水平居右 垂直居中
	//第四行
	mainLayout->addWidget(_login, 3, 1, 1, 2);//登录按钮
	// 设置水平间距
	mainLayout->setHorizontalSpacing(5);
	// 设置垂直间距
	mainLayout->setVerticalSpacing(10);
	// 设置外间距
	mainLayout->setContentsMargins(10, 10, 10, 10);

	connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));
}

void LoginGUI::signInFromNet(QByteArray buf)
{
	QByteArray temp;
	switch (buf.at(1)) {
	case 1:		
		 temp.append(buf.right(buf.size() - 2));
		_userInfo = new UserInformation(UserInformation::DeSerializable(temp));
		QMessageBox::information(this, "提醒","欢迎回来");
		accept();
		break;
	case 2:
		QMessageBox::information(this, "提醒","密码错误");
		break;
	case 3:
		QMessageBox::information(this,"提醒","账号不存在");
		break;
	default:
		break;
	}
}

bool LoginGUI::inputCheck()
{
	if (_accText->text() == "")
	{
		QMessageBox::information(this, "提醒", "账号不能为空");
		return false;
	}
	if (_pwdText->text() == "")
	{
		QMessageBox::information(this, "提醒", "密码不能为空");
		return false;
	}
	if (this->_tempAccount == this->_accText->text() && this->_tempPassword == this->_pwdText->text())
	{
		QMessageBox::information(this, "提醒", "请勿重复输入");
		return false;
	}
	return true;
}

void LoginGUI::slotSignUp()
{
	_registerGUI = new RegisterGUI(_socket);
	disconnect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));
	_registerGUI->exec();
	_registerGUI->disconnect(_socket, SIGNAL(readyRead()), _registerGUI, SLOT(slotDataArrive()));
	delete _registerGUI;
	connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));

}

void LoginGUI::slotDataArrive()
{
	QByteArray buf = _socket->readAll();
	if(buf.at(0)==1)
		signInFromNet(buf);

}

void LoginGUI::slotSignIn()
{
	if (inputCheck())
	{
		this->_tempAccount = this->_accText->text();
		this->_tempPassword = this->_pwdText->text();
	}
	else
		return;

	/* 发送给对方 */
	QByteArray buf;
	buf.append(1);
	buf.append(' ');
	buf.append(_tempAccount);
	buf.append(' ');
	buf.append(_tempPassword);
	_socket->write(buf);
	
}