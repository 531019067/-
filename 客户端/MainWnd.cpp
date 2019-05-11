#include "MainWnd.h"
#include <QHBoxLayout>
#include "SingleGame.h"
#include "MultiGame.h"
#include "NetGame.h"
#include "StepRecord.h"
#pragma execution_character_set("utf-8")
MainWnd::MainWnd(int gameType,RoomInformation *room, QWidget *parent) : QWidget(parent),_currentRoom(room)
{
    _gameType = gameType;

    if(_gameType == 0)//单人，人机对战
    {
        SingleGame* game = new SingleGame;
        CtrlPanel* panel = new CtrlPanel;
		StepRecord*stepList = new StepRecord;
        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game, 1);
        hLay->addWidget(panel);
		hLay->addWidget(stepList);
        connect(panel, SIGNAL(sigBack()), game, SLOT(slotBack()));
		connect(panel, SIGNAL(sigSave()), game, SLOT(slotSave()));
		connect(panel, SIGNAL(sigLoad()), game, SLOT(slotLoad()));
		connect(panel, SIGNAL(sigNew()), stepList, SLOT(slotClear()));
		connect(panel, SIGNAL(sigNew()), game, SLOT(slotNew()));
		connect(game, SIGNAL(sigMove(QString)), stepList, SLOT(slotAddRecord(QString)));
		connect(game, SIGNAL(sigBack()), stepList, SLOT(slotBack()));


		
    }
    else if(_gameType == 1)//两人对战
    {
        MultiGame* game = new MultiGame;
        CtrlPanel* panel = new CtrlPanel;

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game, 1);
        hLay->addWidget(panel);
        connect(panel, SIGNAL(sigBack()), game, SLOT(slotBack()));

		StepRecord*stepList = new StepRecord;
		hLay->addWidget(stepList);
		connect(game, SIGNAL(sigMove(QString)), stepList, SLOT(slotAddRecord(QString)));
		connect(game, SIGNAL(sigBack()), stepList, SLOT(slotBack()));
    }
    else if(_gameType == 2)//服务端
    {
        NetGame* game = new NetGame(true);
        CtrlPanel* panel = new CtrlPanel;

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game, 1);
        hLay->addWidget(panel);
        connect(panel, SIGNAL(sigBack()), game, SLOT(slotBack()));
		StepRecord*stepList = new StepRecord;
		hLay->addWidget(stepList);
		connect(game, SIGNAL(sigMove(QString)), stepList, SLOT(slotAddRecord(QString)));
		connect(game, SIGNAL(sigBack()), stepList, SLOT(slotBack()));
    }
    else if(_gameType == 3)//客户端
    {
		QHostAddress ip = QHostAddress("127.0.0.1");
		if (_currentRoom != nullptr)
			ip = _currentRoom->_ipAddress;
        NetGame* game = new NetGame(false,ip);
        CtrlPanel* panel = new CtrlPanel;

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game, 1);
        hLay->addWidget(panel);
        connect(panel, SIGNAL(sigBack()), game, SLOT(slotBack()));
		StepRecord*stepList = new StepRecord;
		hLay->addWidget(stepList);
		connect(game, SIGNAL(sigMove(QString)), stepList, SLOT(slotAddRecord(QString)));
		connect(game, SIGNAL(sigBack()), stepList, SLOT(slotBack()));
    }
}

MainWnd::~MainWnd()
{

}

void MainWnd::closeEvent(QCloseEvent * event)
{
	if (!(QMessageBox::information(this, ("Tips"), ("你是否认输"), ("Yes"), ("No"))))
	{
		this->close();
	}
	emit quitBattle();

}
