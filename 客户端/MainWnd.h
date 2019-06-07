#ifndef MAINWND_H
#define MAINWND_H

#include <QWidget>
#include "Board.h"
#include "CtrlPanel.h"
#include "RoomInformation.h"
#include <QHostAddress>

class MainWnd : public QWidget
{
    Q_OBJECT
public:
    explicit MainWnd(int gameType,RoomInformation * room=nullptr,QWidget *parent = 0);
    ~MainWnd();
	RoomInformation *_currentRoom;
	Board *_game;
    int _gameType;
protected:
	void closeEvent(QCloseEvent *event) override;
signals:
	void quitBattle();
public slots:
};

#endif // MAINWND_H
