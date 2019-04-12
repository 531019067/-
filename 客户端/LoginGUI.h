#ifndef LoginDlg_H
#define LoginDlg_H

#include <QDialog>

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QMessageBox>

#include <QTcpSocket>
#include <QHostAddress>
#include "UserInformation.h"
#include "RegisterGUI.h"
class LoginGUI : public QDialog
{
	Q_OBJECT
public:
	explicit LoginGUI(QTcpSocket *socket,QWidget *parent = 0);
	~LoginGUI();




	//登录对话框的控件
	QLabel *_headPixmap;//头像
	QLabel *_accLabel,* _pwdLabel;//账号密码标签
	QLineEdit *_accText, *_pwdText;//账号密码输入框
	QPushButton *_login, *_signUp,*_retrieve;//登录注册按钮,找回密码按钮
	QCheckBox *_rememberBox, *_autoLoginBox;//记住密码，自动登录选项

	QTcpSocket *_socket;
	UserInformation *_userInfo;
	QString _tempAccount, _tempPassword;
	RegisterGUI *_registerGUI;
	void init();
	void signInFromNet(QByteArray buf);
	UserInformation getUserInfo();
	bool inputCheck();
signals:
public slots:
	void slotSignIn();
	void slotSignUp();
	void slotDataArrive();
};

#endif // LoginDlg_H
