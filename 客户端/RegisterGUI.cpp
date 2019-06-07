#include "RegisterGUI.h"
#pragma execution_character_set("utf-8")
RegisterGUI::RegisterGUI(QTcpSocket *socket, QWidget *parent) :_socket(socket), QDialog(parent),_tempUserName(""),_tempAccount(""),_tempPassword("")
{
	this->setWindowTitle("ע�ᴰ��");
	_headPixmap = new QLabel();
	_userNameLabel = new QLabel("�ǳ�");
	_userNameText = new QLineEdit();
	_accLabel = new QLabel("�˺�");
	_accText = new QLineEdit();
	_pwdLabel = new QLabel("����");
	_pwdText = new QLineEdit();
	_repeatLabel = new QLabel("ȷ������");
	_repeatText = new QLineEdit();
	_signUp = new QPushButton("ע��");
	_cancel = new QPushButton("ȡ��");
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
	_accText->setPlaceholderText("����6�����20���ַ�");
	_pwdText->setPlaceholderText("����8-50����ĸ��������");
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
		QMessageBox::information(this, "����", "�û�������Ϊ��");
		return false;
	}
	if (_accText->text() == "")
	{
		QMessageBox::information(this, "����", "�˺Ų���Ϊ��");
		return false;
	}
	if (_pwdText->text() == "")
	{
		QMessageBox::information(this, "����", "���벻��Ϊ��");
		return false;
	}
	if (_repeatText->text() != _pwdText->text())
	{
		QMessageBox::information(this, "����", "�������벻һ��");
		return false;
	}
	if (_userNameText->text().contains(' '))
	{
		QMessageBox::information(this, "����", "�û��������пո�");
		return false;
	}
	if (_accText->text().contains(' '))
	{
		QMessageBox::information(this, "����", "�˺Ų����пո�");
		return false;
	}
	if (_pwdText->text().contains(' '))
	{
		QMessageBox::information(this, "����", "���벻���пո�");
		return false;
	}

	return true;
}

void RegisterGUI::signUpFromNet(QByteArray buf)
{
	switch (buf.at(1)) {
	case 1:
		QMessageBox::information(this, "����", "ע��ɹ�");
		accept();
		break;
	case 2:
		QMessageBox::information(this, "����", "�˺���ע��");
		break;
	case 3:
		QMessageBox::information(this, "����", "�û����Ѵ���");
		break;
	default:
		break;
	}
}
void RegisterGUI::slotDataArrive()//�յ�����ʱ
{
	QByteArray buf = _socket->readAll();
	if (buf.at(0) == 2)
	{
		signUpFromNet(buf);
	}	
}


void RegisterGUI::slotSignUp()
{
	if (inputCheck())//����û�����Ϸ���
	{
		_tempUserName = _userNameText->text();
		_tempAccount = _accText->text();
		_tempPassword = _pwdText->text();
	}
	else
	{
		return;
	}
	/* ���͸��Է� */
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
