#include "StepRecord.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#pragma execution_character_set("utf-8")
StepRecord::StepRecord(QWidget * parent):QWidget(parent), _redKing(".\\Resources\\RJIANG"),_blackKing(".\\Resources\\BJIANG"),_pageNum(0)
{
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	_turn = new QLabel();
	
	drawTurn();//���Ʊ�ʾ�ֵ��ķ��Ľ���˧����
	mainLayout->addWidget(_turn);

	_model = new QStandardItemModel(15, 1);//15�еı��
	_model->setHeaderData(0, Qt::Horizontal, "�߷�");

	_stepTable = new QTableView(this);
	_stepTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ò��ɱ༭
	_stepTable->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ģʽΪѡ����
	_stepTable->setModel(_model);//��������
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


	//QColor >> QRgb(uint) QRgb��ʵ��һ��typedef�����ڴ��32-bit��������Ϣ��QColor����һ���ṩ�����ӿں������࣬��Qt�й㷺�����ڴ洢ɫ�ʡ�
	QColor c(0, 255, 0);
	QRgb rgb = qRgb(c.red(), c.green(), c.blue());
	QImage mask = stone.createMaskFromColor(rgb, Qt::MaskOutColor);//�����ɰ�
	stone.setAlphaChannel(mask);//���ɰ�����alphaͨ��
	_turn->setPixmap(QPixmap::fromImage(stone));

	
}

void StepRecord::slotBack()
{
	_pageNum = (_recordVector.size() - 1) / 15;//�������һҳ
	_prePage->setEnabled(true);
	_nextPage->setDisabled(true);
	_recordVector.pop_back();
	//�����������15�����һҳ������
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
	//������һҳ����һҳ��Ȼ����
	_prePage->setEnabled(true);
	//��������һҳ����һҳ������
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
	//������һҳ����һҳ��Ȼ����
	_nextPage->setEnabled(true);
	//����ǵ�һҳ����һҳ������
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
	//����������15����һҳ�Ų��£������
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

