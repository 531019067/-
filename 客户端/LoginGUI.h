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




	//��¼�Ի���Ŀؼ�
	QLabel *_headPixmap;//ͷ��
	QLabel *_accLabel,* _pwdLabel;//�˺������ǩ
	QLineEdit *_accText, *_pwdText;//�˺����������
	QPushButton *_login, *_signUp,*_retrieve;//��¼ע�ᰴť,�һ����밴ť
	QCheckBox *_rememberBox, *_autoLoginBox;//��ס���룬�Զ���¼ѡ��

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
