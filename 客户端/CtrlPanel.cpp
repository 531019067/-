#include "CtrlPanel.h"
#include <QGridLayout>
#include <QVBoxLayout>
#pragma execution_character_set("utf-8")
CtrlPanel::CtrlPanel(QWidget *parent) : QWidget(parent)
{
    QGridLayout* grid = new QGridLayout(this);
    QVBoxLayout* vLay = new QVBoxLayout;
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(2, 1);
    grid->setRowStretch(0, 1);
    grid->setRowStretch(2, 1);
    grid->addLayout(vLay, 1, 1);
	vLay->addWidget(this->_newBattle = new QPushButton("�¾�"));
	vLay->addWidget(this->_saveBattle = new QPushButton("����Ծ�"));
	vLay->addWidget(this->_loadBattle = new QPushButton("���ضԾ�"));
    vLay->addWidget(this->_back = new QPushButton("����"));
	vLay->addWidget(this->_giveUp = new QPushButton("����"));
	vLay->addWidget(this->_suePeace = new QPushButton("���"));
	vLay->addWidget(this->_menu = new QPushButton("�˵�"));
	connect(this->_newBattle, SIGNAL(clicked()), this, SIGNAL(sigNew()));
    connect(this->_back, SIGNAL(clicked()), this, SIGNAL(sigBack()));
	connect(this->_loadBattle, SIGNAL(clicked()), this, SIGNAL(sigLoad()));
	connect(this->_saveBattle, SIGNAL(clicked()), this, SIGNAL(sigSave()));
	connect(this->_giveUp, SIGNAL(clicked()), this, SIGNAL(sigGive()));
	connect(this->_suePeace, SIGNAL(clicked()), this, SIGNAL(sigPeace()));

}

CtrlPanel::~CtrlPanel()
{

}

