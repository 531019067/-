#include "StepRecord.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#pragma execution_character_set("utf-8")
StepRecord::StepRecord(QWidget * parent):QWidget(parent), _redKing(".\\Resources\\RJIANG"),_blackKing(".\\Resources\\BJIANG"),_pageNum(0)
{
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	_turn = new QLabel();
	
	drawTurn();//绘制表示轮到哪方的将（帅）字
	mainLayout->addWidget(_turn);

	_model = new QStandardItemModel(15, 1);//15行的表格
	_model->setHeaderData(0, Qt::Horizontal, "走法");

	_stepTable = new QTableView(this);
	_stepTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
	_stepTable->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
	_stepTable->setModel(_model);//挂载数据
	_stepTable->setColumnWidth(0, 85);
	_stepTable->setFixedSize(100, 480);
	mainLayout->addWidget(_stepTable);

	QHBoxLayout *control = new QHBoxLayout();
	_prePage=new QPushButton("<<");
	connect(_prePage, SIGNAL(clicked()), this, SLOT(slotPreviousPage()));
	_nextPage = new QPushButton(">>");
	connect(_nextPage, SIGNAL(clicked()), this, SLOT(slotNextPage()));
	control->addWidget(_prePage);
	control->addWidget(_nextPage);
	_prePage->setDisabled(true);
	_nextPage->setDisabled(true);
	mainLayout->addLayout(control);

	_bRedTurn = true;
}

void StepRecord::drawTurn()
{
	QPoint pos(10, 10);
	QImage stone;
	if (_bRedTurn)
		stone.load(".\\Resources\\RK");
	else
		stone.load(".\\Resources\\BK");


	//QColor >> QRgb(uint) QRgb其实是一个typedef，用于存放32-bit的像素信息。QColor则是一个提供了许多接口函数的类，在Qt中广泛的用于存储色彩。
	QColor c(0, 255, 0);
	QRgb rgb = qRgb(c.red(), c.green(), c.blue());
	QImage mask = stone.createMaskFromColor(rgb, Qt::MaskOutColor);//创建蒙版
	stone.setAlphaChannel(mask);//用蒙版设置alpha通道
	_turn->setPixmap(QPixmap::fromImage(stone));

	
}

void StepRecord::slotBack()
{
	_pageNum = (_recordVector.size() - 1) / 15;//返回最后一页
	_prePage->setEnabled(true);
	_nextPage->setDisabled(true);
	_recordVector.pop_back();
	//如果数组少于15项，则上一页不可用
	if (_recordVector.size() < 15)
	{
		_nextPage->setEnabled(false);
		_prePage->setEnabled(false);
	}
	for (int i = 0; i < 15; i++)
	{
		if ((_pageNum * 15 + i) < _recordVector.size())
			_model->setItem(i, new QStandardItem(_recordVector[_pageNum * 15 + i]));
		else
			_model->setItem(i, 0);
	}
	_bRedTurn = !_bRedTurn;
	drawTurn();
}

void StepRecord::slotClear()
{
	_pageNum = 0;
	_prePage->setDisabled(true);
	_nextPage->setDisabled(true);
	for (int i = 0; i < 15; i++)
		_model->setItem(i, 0);
	_recordVector.clear();
}

void StepRecord::slotNextPage()
{
	_pageNum++;
	//访问下一页，上一页必然可用
	_prePage->setEnabled(true);
	//如果是最后一页，下一页不可用
	if(_pageNum== (_recordVector.size() - 1) / 15)
		_nextPage->setDisabled(true);
	for (int i = 0; i < 15; i++)
	{
		if((_pageNum * 15 + i)<_recordVector.size())
			_model->setItem(i, new QStandardItem(_recordVector[_pageNum * 15 + i]));
		else
			_model->setItem(i, 0);
	}
}

void StepRecord::slotPreviousPage()
{
	_pageNum--;
	//返回上一页则下一页必然可用
	_nextPage->setEnabled(true);
	//如果是第一页，上一页不可用
	if (_pageNum == 0)
		_prePage->setDisabled(true);
		
	for (int i = 0; i < 15; i++)
	{
		if ((_pageNum * 15 + i) < _recordVector.size())
			_model->setItem(i, new QStandardItem(_recordVector[_pageNum * 15 + i]));
		else
			_model->setItem(i, 0);
	}
}

void StepRecord::slotAddRecord(QString record)
{
	_recordVector.push_back(record);
	//当步数超过15步，一页放不下，则可用
	if (_recordVector.size() >= 15)
		_prePage->setEnabled(true);

	_pageNum = (_recordVector.size()-1) / 15;
	for (int i = 0; i < 15; i++)
	{
		if ((_pageNum * 15 + i) < _recordVector.size())
			_model->setItem(i, new QStandardItem(_recordVector[_pageNum * 15 + i]));
		else
			_model->setItem(i, 0);
	}


	_bRedTurn=!_bRedTurn;
	drawTurn();
}

