#pragma once

#include <QWidget>
#include <QPushButton>


class CtrlBattlePanel :public QWidget
{
public:
	Q_OBJECT
public:
	explicit CtrlBattlePanel(QWidget *parent = 0);
	~CtrlBattlePanel();

	QPushButton* _createBattle;
	QPushButton* _joinBattle;
signals:
	void sigCreate();
	void sigJoin();

public slots:

};