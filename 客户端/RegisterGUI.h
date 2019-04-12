#pragma once

#include <QDialog>

#include <QVboxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

#include <QTcpSocket>
#include <QHostAddress>

class RegisterGUI : public QDialog
{
	Q_OBJECT
public:
	explicit RegisterGUI(QTcpSocket *socket, QWidget *parent = 0);
	~RegisterGUI();




	//登录对话框的控件
	QLabel *_headPixmap;//头像
	QLabel *_userNameLabel,*_accLabel, *_pwdLabel,*_repeatLabel;//账号密码,重复密码
	QLineEdit *_userNameText,*_accText, *_pwdText,*_repeatText;//账号密码输入框
	QPushButton *_signUp, *_cancel;//注册按钮,取消按钮


	QTcpSocket *_socket;

	QString _tempUserName,_tempAccount, _tempPassword;

	void init();
	bool inputCheck();
	void signUpFromNet(QByteArray buf);

signals:
public slots:
	void slotDataArrive();
	void slotSignUp();
	
	
};

