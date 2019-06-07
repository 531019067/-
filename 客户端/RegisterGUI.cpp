#include "RegisterGUI.h"
#pragma execution_character_set("utf-8")
RegisterGUI::RegisterGUI(QTcpSocket *socket, QWidget *parent) :_socket(socket), QDialog(parent),_tempUserName(""),_tempAccount(""),_tempPassword("")
{
	this->setWindowTitle("注册窗口");
	_headPixmap = new QLabel();
	_userNameLabel = new QLabel("昵称");
	_userNameText = new QLineEdit();
	_accLabel = new QLabel("账号");
	_accText = new QLineEdit();
	_pwdLabel = new QLabel("密码");
	_pwdText = new QLineEdit();
	_repeatLabel = new QLabel("确定密码");
	_repeatText = new QLineEdit();
	_signUp = new QPushButton("注册");
	_cancel = new QPushButton("取消");
	connect(_signUp, SIGNAL(clicked()), this, SLOT(slotSignUp()));
	connect(_cancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));
	init();
}
RegisterGUI::~RegisterGUI()
{

}
void RegisterGUI::init()
{
	_accText->setPlaceholderText("最少6个最多20个字符");
	_pwdText->setPlaceholderText("长度8-50个字母或者数字");
	QHBoxLayout *line = new QHBoxLayout[5];
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	line[0].addWidget(_userNameLabel);
	line[0].addWidget(_userNameText);
	line[1].addWidget(_accLabel);
	line[1].addWidget(_accText);
	line[2].addWidget(_pwdLabel);
	line[2].addWidget(_pwdText);
	line[3].addWidget(_repeatLabel);
	line[3].addWidget(_repeatText);
	line[4].addWidget(_signUp);
	line[4].addWidget(_cancel);
	for (int i=0;i<5;i++)
	{
		mainLayout->addLayout(&line[i]);
	}
	
	
}

bool RegisterGUI::inputCheck()
{
	if (_userNameText->text() == "")
	{
		QMessageBox::information(this, "提醒", "用户名不能为空");
		return false;
	}
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
	if (_repeatText->text() != _pwdText->text())
	{
		QMessageBox::information(this, "提醒", "两次密码不一样");
		return false;
	}
	if (_userNameText->text().contains(' '))
	{
		QMessageBox::information(this, "提醒", "用户名不能有空格");
		return false;
	}
	if (_accText->text().contains(' '))
	{
		QMessageBox::information(this, "提醒", "账号不能有空格");
		return false;
	}
	if (_pwdText->text().contains(' '))
	{
		QMessageBox::information(this, "提醒", "密码不能有空格");
		return false;
	}

	return true;
}

void RegisterGUI::signUpFromNet(QByteArray buf)
{
	switch (buf.at(1)) {
	case 1:
		QMessageBox::information(this, "提醒", "注册成功");
		accept();
		break;
	case 2:
		QMessageBox::information(this, "提醒", "账号已注册");
		break;
	case 3:
		QMessageBox::information(this, "提醒", "用户名已存在");
		break;
	default:
		break;
	}
}
void RegisterGUI::slotDataArrive()//收到数据时
{
	QByteArray buf = _socket->readAll();
	if (buf.at(0) == 2)
	{
		signUpFromNet(buf);
	}	
}


void RegisterGUI::slotSignUp()
{
	if (inputCheck())//检查用户输入合法性
	{
		_tempUserName = _userNameText->text();
		_tempAccount = _accText->text();
		_tempPassword = _pwdText->text();
	}
	else
	{
		return;
	}
	/* 发送给对方 */
	QByteArray buf;
	buf.append(2);
	buf.append(' ');
	buf.append(_tempUserName.trimmed());
	buf.append(' ');
	buf.append(_tempAccount.trimmed());
	buf.append(' ');
	buf.append(_tempPassword.trimmed());
	_socket->write(buf);
}
