#include "CtrlPanel.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include "LoadDlg.h"
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QDebug>
#pragma execution_character_set("utf-8")
CtrlPanel::CtrlPanel(Board &game,QWidget *parent) : QWidget(parent),_game(&game)
{
    QGridLayout* grid = new QGridLayout(this);
    QVBoxLayout* vLay = new QVBoxLayout;
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(2, 1);
    grid->setRowStretch(0, 1);
    grid->setRowStretch(2, 1);
    grid->addLayout(vLay, 1, 1);
	vLay->addWidget(this->_newBattle = new QPushButton("新局"));
	vLay->addWidget(this->_saveBattle = new QPushButton("保存对局"));
	vLay->addWidget(this->_loadBattle = new QPushButton("加载对局"));
    vLay->addWidget(this->_back = new QPushButton("悔棋"));
	vLay->addWidget(this->_giveUp = new QPushButton("认输"));
	vLay->addWidget(this->_suePeace = new QPushButton("求和"));
	vLay->addWidget(this->_menu = new QPushButton("菜单"));
	connect(this->_newBattle, SIGNAL(clicked()), this, SLOT(slotNew()));
    connect(this->_back, SIGNAL(clicked()), this, SLOT(slotBack()));
	connect(this->_loadBattle, SIGNAL(clicked()), this, SLOT(slotLoad()));
	connect(this->_saveBattle, SIGNAL(clicked()), this, SLOT(slotSave()));
	connect(this->_menu, SIGNAL(clicked()), this, SLOT(slotMenu()));
	/*connect(this->_giveUp, SIGNAL(clicked()), this, SIGNAL(slotGive()));
	connect(this->_suePeace, SIGNAL(clicked()), this, SIGNAL(slotPeace()));*/

}

CtrlPanel::~CtrlPanel()
{

}


void CtrlPanel::slotSave()
{
	QFile fenFile(".\\Fen.txt");
	if (!fenFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		qDebug() << "Could not open file for writing";
		return;
	}
	QTextStream out(&fenFile);
	bool isOk;
	QString name = QInputDialog::getText(this, "保存对局", "请命名对局", QLineEdit::Normal, nullptr, &isOk);
	if (isOk)
	{
		QString fen = _game->_situation.createFen();
		out << name << ':' << fen << '\n';
		QMessageBox::information(this, "tips", "保存成功");
	}
	fenFile.close();

}

void CtrlPanel::slotBack()
{
	_game->back();
}

void CtrlPanel::slotMenu()
{
	QMessageBox::information(this, "tips", "红方长捉,不变判负");
}

void CtrlPanel::slotNew()
{
	_game->init(_game->_bRedSide);
	emit sigCleanRecord(_game->_bRedTurn);
}

void CtrlPanel::slotLoad()
{
	LoadDlg * loadDlg = new LoadDlg(this);
	loadDlg->loadFile(".\\Fen.txt");
	if (loadDlg->exec() == QDialog::Accepted)
	{
		QString fen = loadDlg->_fenList->at(loadDlg->_selected);
		_game->load(fen);
		_game->_situation.loadFromFen(fen);
	}
	emit sigCleanRecord(_game->_bRedTurn);

}

