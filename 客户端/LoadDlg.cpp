#include "LoadDlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QDebug>
#include <QDataStream>
#pragma execution_character_set("utf-8")
LoadDlg::LoadDlg(QWidget *parent /* = 0 */) :QDialog(parent)
{
	_fenList = new QVector<QString>;
	_battleList=new QListWidget(this);
	_confirm = new QPushButton("确定");
	_cancel = new QPushButton("取消");

	QVBoxLayout *mainLyaout = new QVBoxLayout(this);
	QHBoxLayout *buttonLyaout = new QHBoxLayout();
	mainLyaout->addWidget(_battleList);
	buttonLyaout->addWidget(_confirm);
	buttonLyaout->addWidget(_cancel);
	mainLyaout->addLayout(buttonLyaout);
	
	connect(_confirm, SIGNAL(clicked()), this, SLOT(slotConfirm()));
	connect(_cancel, SIGNAL(clicked()), this, SLOT(slotCancel()));

}
LoadDlg::~LoadDlg()
{

}

void LoadDlg::loadFile(QString fileName)
{
	QFile fenFile(fileName);
	if (!fenFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "there is not file";
	}
	QTextStream in(&fenFile);
	while (!in.atEnd())
	{
		QString data = in.readLine();
		qDebug() << data;
		//分割字符串，提取名字和fen串
		QString name = data.section(':', 0, 0);
		QString fen = data.section(':', 1, 1);
		QListWidgetItem * battle = new QListWidgetItem(name);
		_fenList->append(fen);
		_battleList->addItem(battle);
		
	}
	fenFile.close();
}
void LoadDlg::slotConfirm()
{
	_selected = _battleList->currentRow();
	accept();
}
void LoadDlg::slotCancel()
{
	reject();
}
