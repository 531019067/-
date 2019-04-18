#include "CtrlBattlePanel.h"
#include <QHBoxLayout>
#pragma execution_character_set("utf-8")
CtrlBattlePanel::CtrlBattlePanel(QWidget *parent) :QWidget(parent)
{
	_createBattle = new QPushButton("创建对战");
	_joinBattle = new QPushButton("加入对战");
	QHBoxLayout *p = new QHBoxLayout(this);
	connect(_createBattle, SIGNAL(clicked()), this, SIGNAL(sigCreate()));
	connect(_joinBattle, SIGNAL(clicked()), this, SIGNAL(sigJoin()));
	p->addWidget(_createBattle);
	p->addWidget(_joinBattle);
}
CtrlBattlePanel::~CtrlBattlePanel()
{
}
