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




	//��¼�Ի���Ŀؼ�
	QLabel *_headPixmap;//ͷ��
	QLabel *_userNameLabel,*_accLabel, *_pwdLabel,*_repeatLabel;//�˺�����,�ظ�����
	QLineEdit *_userNameText,*_accText, *_pwdText,*_repeatText;//�˺����������
	QPushButton *_signUp, *_cancel;//ע�ᰴť,ȡ����ť


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

