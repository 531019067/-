#pragma once
#include <QWidget>

#include <QPixmap>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QVector>
#include <QList>
#include <QLabel>
#include "Step.h"
#include "Stone.h"
using namespace std;
class StepRecord :public QWidget
{
	Q_OBJECT;
public:
	explicit StepRecord(QWidget *parent=0);
	void drawTurn();
	//void drawTable();

public slots:
	void slotAddRecord(QString record);
	void slotBack();
	void slotClear(bool turn=true);
	void slotNextPage();
	void slotPreviousPage();
private:
	QPushButton * _prePage, *_nextPage;
	int _pageNum;
	QPixmap _redKing,_blackKing;
	QLabel *_turn;
	QTableView * _stepTable;
	QStandardItemModel *_model;
	QVector<QString> _recordVector;
	
	bool _bRedTurn;

};