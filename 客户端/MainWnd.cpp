#include "MainWnd.h"
#include <QHBoxLayout>
#include "SingleGame.h"
#include "MultiGame.h"
#include "NetGame.h"
#include "StepRecord.h"
#include <QCloseEvent>
#pragma execution_character_set("utf-8")
MainWnd::MainWnd(int gameType,RoomInformation *room, QWidget *parent) : QWidget(parent),_currentRoom(room)
{
    _gameType = gameType;

    if(_gameType == 0)//���ˣ��˻���ս
    {
        _game = new SingleGame;
    }
    else if(_gameType == 1)//���˶�ս
    {
        _game = new MultiGame;
    }
    else if(_gameType == 2)//�����
    {
		_game = new NetGame(true);
    }
    else if(_gameType == 3)//�ͻ���
    {
		QHostAddress ip = QHostAddress("127.0.0.1");
		if (_currentRoom != nullptr)
			ip = _currentRoom->_ipAddress;
        _game = new NetGame(false,ip);
    }
	CtrlPanel* panel = new CtrlPanel(*_game);
	StepRecord*stepList = new StepRecord;
	QHBoxLayout* hLay = new QHBoxLayout(this);
	hLay->addWidget(_game, 1);
	hLay->addWidget(panel);
	hLay->addWidget(stepList);
	connect(panel, SIGNAL(sigCleanRecord(bool)), stepList, SLOT(slotClear(bool)));
	connect(_game, SIGNAL(sigMove(QString)), stepList, SLOT(slotAddRecord(QString)));
	connect(_game, SIGNAL(sigBack()), stepList, SLOT(slotBack()));
}

MainWnd::~MainWnd()
{

}

void MainWnd::closeEvent(QCloseEvent * event)
{
	int button = QMessageBox::question(this, ("Tips"), ("���Ƿ�����"), QMessageBox::Yes | QMessageBox::No);
	switch (button)
	{
	case QMessageBox::No:
		event->ignore(); // �����˳��źţ������������
		break;
	case QMessageBox::Yes:
		emit quitBattle();
		event->accept();
		break;
	default:
		break;
	}
	

}
