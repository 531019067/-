#include "MainWnd.h"
#include <QHBoxLayout>
#include "SingleGame.h"
#include "MultiGame.h"
#include "NetGame.h"

MainWnd::MainWnd(int gameType,RoomInformation *room, QWidget *parent) : QWidget(parent),_currentRoom(room)
{
    _gameType = gameType;

    if(_gameType == 0)//单人，人机对战
    {
        SingleGame* game = new SingleGame;
        CtrlPanel* panel = new CtrlPanel;

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game, 1);
        hLay->addWidget(panel);
        connect(panel, SIGNAL(sigBack()), game, SLOT(slotBack()));
    }
    else if(_gameType == 1)//多人对战
    {
        MultiGame* game = new MultiGame;
        CtrlPanel* panel = new CtrlPanel;

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game, 1);
        hLay->addWidget(panel);
        connect(panel, SIGNAL(sigBack()), game, SLOT(slotBack()));
    }
    else if(_gameType == 2)//服务端
    {
        NetGame* game = new NetGame(true);
        CtrlPanel* panel = new CtrlPanel;

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game, 1);
        hLay->addWidget(panel);
        connect(panel, SIGNAL(sigBack()), game, SLOT(slotBack()));
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
    }
}

MainWnd::~MainWnd()
{

}

void MainWnd::closeEvent(QCloseEvent * event)
{
	if (!(QMessageBox::information(this, tr("CT Control View"), tr("Do you really want to log out CT Control View?"), tr("Yes"), tr("No"))))
	{
		this->close();
	}
	emit quitBattle();

}

