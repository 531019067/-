#ifndef CTRLPANEL_H
#define CTRLPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "Board.h"
class CtrlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CtrlPanel(Board&game,QWidget *parent = 0);
    ~CtrlPanel();
	Board *_game;
    QPushButton* _back;
	QPushButton* _newBattle;
	QPushButton* _saveBattle;
	QPushButton* _loadBattle;
	QPushButton* _giveUp;
	QPushButton* _suePeace;
	QPushButton* _menu;

signals:
	void sigCleanRecord(bool turn);

public slots:
	void slotNew();
	void slotLoad();
	void slotSave();
	void slotBack();
	void slotMenu();
};

#endif // CTRLPANEL_H
