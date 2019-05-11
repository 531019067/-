#include "Board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QInputDialog>
#include <QListWidgetItem>
#include "LoadDlg.h"
#pragma execution_character_set("utf-8")
#define GetRowCol(__row, __col, __id) \
    int __row = _s[__id]._row;\
    int __col = _s[__id]._col

Board::Board(QWidget *parent) : QFrame(parent)
{
	_r = 28;//半径，棋子图片为56*56
	_d = 57;//棋盘间隔，棋盘每个交叉点间隔57
	_off = QPoint(33, 32);//棋盘左上角到第一个棋子中心点的偏移
	_pos = QPoint(10, 10);//棋盘左上角到窗体左上角的距离。
	setMinimumSize(_d * 9 + 35, _d * 10 + 30);
	init(true);


}

Board::~Board()
{

}

void Board::init(bool bRedSide)//初始化棋局
{
	for (int i = 0; i < 32; ++i)
	{
		_s[i].init(i);// id小于16时，为红棋，初始时红上黑下
	}
	if (bRedSide)//是否为红方
	{
		for (int i = 0; i < 32; ++i)
		{
			_s[i].rotate();//翻转棋子，红下黑上
		}
	}

	_selectid = -1;
	_bRedTurn = true;
	_bRedSide = bRedSide;
	update();
}

int Board::getType(char stone)
{
	switch (stone)
	{
	case 'K':
		return 0;
	case 'A':
		return 1;
	case 'B':
		return 2;
	case 'N':
		return 3;
	case 'R':
		return 4;
	case 'C':
		return 5;
	case 'P':
		return 6;
	default:
		return 7;
	}
}

void Board::load(QString fen)
{
	int redStoneId[7], blackStoneId[7];
	redStoneId[0] = 0;//帅0
	redStoneId[1] = 1;//士1 2
	redStoneId[2] = 3;//象3 4
	redStoneId[3] = 5;//马5 6
	redStoneId[4] = 7;//车7 8 
	redStoneId[5] = 9;//炮9 10
	redStoneId[6] = 11;//兵11-15
	for (int i=0;i<7;i++)
	{
		blackStoneId[i] = redStoneId[i] + 16;
	}
	//清空所有棋子
	for (int i = 0; i < 32; i++)
	{
		_s[i]._dead = true;
	}
  	QString situation = fen.section(' ', 0, 0);
	_bRedTurn = fen.section(' ', 1, 1) != 'b';
	int row = 0, col = 0;
	for (int i = 0; i < situation.size(); i++)
	{
		if (situation[i] == '/')
		{
			row++;
			col = 0;
		}
		if (situation[i].isLower())
		{
			int type = getType(situation[i].toUpper().unicode());//红方棋子id从0-15;
			int index = redStoneId[type];
			redStoneId[type]++;
			_s[index]._row = row;
			_s[index]._col = col;
			_s[index]._dead = false;
			col++;
			

		}
		if (situation[i].isUpper())
		{
			int type = getType(situation[i].unicode());//黑方棋子id从16-31;
			int index = blackStoneId[type];
			blackStoneId[type]++;
			_s[index]._row = row;
			_s[index]._col = col;
			_s[index]._dead = false;
			col++;
		}
		if (situation[i].isDigit())
		{
			col += situation[i].unicode() - 48;
		}
	}
}

void Board::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	p.save();
	drawPlate(p);
	p.restore();

	p.save();
	drawStone(p);
	p.restore();

	drawSelected(p, _selectid);
}

void Board::drawPlate(QPainter & p) //绘制棋盘
{
	QPixmap board(".\\Resources\\BOARD");
	p.drawPixmap(_pos, board);//绘制棋盘
}

void Board::drawStone(QPainter &p)//绘制棋子
{
	for (int i = 0; i < 32; i++)
	{
		drawStone(p, i);
	}
}
void Board::drawStone(QPainter &p, int id)
{
	if (isDead(id)) return;

	QImage stone(_s[id].bmpDir());
	//QColor >> QRgb(uint) QRgb其实是一个typedef，用于存放32-bit的像素信息。QColor则是一个提供了许多接口函数的类，在Qt中广泛的用于存储色彩。
	QColor c(0, 255, 0);
	QRgb rgb = qRgb(c.red(), c.green(), c.blue());
	QImage mask = stone.createMaskFromColor(rgb, Qt::MaskOutColor);//创建蒙版
	stone.setAlphaChannel(mask);//用蒙版设置alpha通道
	p.drawImage(topLeft(id), stone);//绘制图片
}

void Board::drawSelected(QPainter & p, int id)
{

	if (id == -1) return;
	QImage selected(".\\Resources\\SELECTED");
	//QColor >> QRgb(uint) QRgb其实是一个typedef，用于存放32-bit的像素信息。QColor则是一个提供了许多接口函数的类，在Qt中广泛的用于存储色彩。
	QColor c(0, 255, 0);
	QRgb rgb = qRgb(c.red(), c.green(), c.blue());
	QImage mask = selected.createMaskFromColor(rgb, Qt::MaskOutColor);//创建蒙版
	selected.setAlphaChannel(mask);//用蒙版设置alpha通道
	p.drawImage(topLeft(id), selected);//绘制图片
}

QString Board::name(int id)
{
	return _s[id].name();
}

bool Board::red(int id)
{
	return _s[id]._red;
}


QPoint Board::center(int row, int col)
{
	QPoint pt(_d*col, _d*row);
	return pt + _off + _pos;
}

QPoint Board::center(int id)
{
	return center(_s[id]._row, _s[id]._col);
}

QPoint Board::topLeft(int row, int col)
{
	return center(row, col) - QPoint(_r, _r);
}

QPoint Board::topLeft(int id)
{
	return center(id) - QPoint(_r, _r);
}

QRect Board::cell(int row, int col)
{
	return QRect(topLeft(row, col), QSize(_r * 2 - 1, _r * 2 - 1));
}

QRect Board::cell(int id)
{
	return QRect(topLeft(id), QSize(_r * 2 - 1, _r * 2 - 1));
}

bool Board::isDead(int id)
{
	if (id == -1)return true;
	return _s[id]._dead;
}

void Board::recordAddOrder(QString & record, bool stoneColor,int i)
{
	if (_bRedSide && stoneColor)
		record += RedOrder[i];
	if (_bRedSide && !stoneColor)
		record += BlackOrder[i];
	if (!_bRedSide &&  stoneColor)
		record += RedOrder[8 - i];
	if (!_bRedSide && !stoneColor)
		record += BlackOrder[8 - i];
}

QString Board::createRecord(Step * step)
{
	QString record = "";
	int id = step->_moveid;
	int id2;
	Stone::TPYE type = _s[id]._type;
	bool isRedBottom = _bRedSide == _s[id]._red;

	if (type == Stone::ADVISOR || type == Stone::KNIGHT || type == Stone::ROOK || type == Stone::CANNON)
	{
		if (id % 2 == 0)
			id2 = id - 1;
		else
			id2 = id + 1;

		if (_s[id]._col == _s[id2]._col && !_s[id2]._dead)//当同一列还有相同棋子
		{
			if ((_s[id]._row > _s[id2]._row) ^ (_bRedSide == _s[id]._red))
				record += "前" + _s[id].name();
			else
				record += "后" + _s[id].name();

		}
	}
	if (record.size() == 0)
	{
		record += _s[id].name();
		recordAddOrder(record, _s[id]._red, step->_colFrom);
	}
	int rowDst = step->_rowTo - step->_rowFrom;
	if (rowDst == 0)//行距为0 ，即横着走
	{
		record += "平";
		recordAddOrder(record, _s[id]._red, step->_colTo);
		return record;
	}
	//行距大于0表示棋子从上往下走
	if (rowDst > 0 ^ (_bRedSide == _s[id]._red))
		record += "进";
	else
		record += "退";
	switch (type)
	{
		//走直线的棋子
	case Stone::KING:
	case Stone::PAWN:
	case Stone::ROOK:
	case Stone::CANNON:
		record += BlackOrder[qAbs(rowDst)-1];
		break;
		//走斜线的棋子
	case Stone::KNIGHT:
	case Stone::ADVISOR:
	case Stone::BISHOP:
		recordAddOrder(record, _s[id]._red, step->_colTo);
		break;
	}
	return record;
}



bool Board::getClickRowCol(QPoint pt, int &row, int &col)
{
	for (row = 0; row <= 9; ++row)
	{
		for (col = 0; col <= 8; ++col)
		{
			QPoint distance = center(row, col) - pt;
			if (distance.x() * distance.x() + distance.y() * distance.y() < _r* _r)
				return true;
		}
	}
	return false;
}

void Board::mouseReleaseEvent(QMouseEvent *ev)
{
	if (ev->button() != Qt::LeftButton)
	{
		return;
	}

	click(ev->pos());
}

bool Board::canSelect(int id)
{
	return _bRedTurn == _s[id]._red;
}

void Board::trySelectStone(int id)
{
	if (id == -1)
		return;

	if (!canSelect(id)) return;

	_selectid = id;
	update();
}

bool Board::sameColor(int id1, int id2)
{
	if (id1 == -1 || id2 == -1) return false;

	return red(id1) == red(id2);
}

void Board::tryMoveStone(int killid, int row, int col)
{
	if (killid != -1 && sameColor(killid, _selectid))
	{
		trySelectStone(killid);
		return;
	}

	bool ret = canMove(_selectid, killid, row, col);
	if (ret)
	{
		actualMoveStone(_selectid, killid, row, col);
		_selectid = -1;
		update();
	}
}

int Board::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
	int ret = 0;
	if (row1 != row2 && col1 != col2)
		return -1;
	if (row1 == row2 && col1 == col2)
		return -1;

	if (row1 == row2)
	{
		int min = col1 < col2 ? col1 : col2;
		int max = col1 < col2 ? col2 : col1;
		for (int col = min + 1; col < max; ++col)
		{
			if (getStoneId(row1, col) != -1) ++ret;
		}
	}
	else
	{
		int min = row1 < row2 ? row1 : row2;
		int max = row1 < row2 ? row2 : row1;
		for (int row = min + 1; row < max; ++row)
		{
			if (getStoneId(row, col1) != -1) ++ret;
		}
	}

	return ret;
}

int Board::relation(int row1, int col1, int row, int col)
{
	return qAbs(row1 - row) * 10 + qAbs(col1 - col);
}

bool Board::canMoveChe(int moveid, int, int row, int col)
{
	GetRowCol(row1, col1, moveid);
	int ret = getStoneCountAtLine(row1, col1, row, col);
	if (ret == 0)
		return true;
	return false;
}

bool Board::canMoveMa(int moveid, int, int row, int col)
{
	GetRowCol(row1, col1, moveid);
	int r = relation(row1, col1, row, col);
	if (r != 12 && r != 21)
		return false;

	if (r == 12)
	{
		if (getStoneId(row1, (col + col1) / 2) != -1)
			return false;
	}
	else
	{
		if (getStoneId((row + row1) / 2, col1) != -1)
			return false;
	}

	return true;
}

bool Board::canMovePao(int moveid, int killid, int row, int col)
{
	GetRowCol(row1, col1, moveid);
	int ret = getStoneCountAtLine(row, col, row1, col1);
	if (killid != -1)
	{
		if (ret == 1) return true;
	}
	else
	{
		if (ret == 0) return true;
	}
	return false;
}

bool Board::canMoveBing(int moveid, int, int row, int col)
{
	GetRowCol(row1, col1, moveid);
	int r = relation(row1, col1, row, col);
	if (r != 1 && r != 10) return false;

	if (isBottomSide(moveid))
	{
		if (row > row1) return false;
		if (row1 >= 5 && row == row1) return false;
	}
	else
	{
		if (row < row1) return false;
		if (row1 <= 4 && row == row1) return false;
	}

	return true;
}

bool Board::canMoveJiang(int moveid, int killid, int row, int col)
{
	if (killid != -1 && _s[killid]._type == Stone::KING)
		return canMoveChe(moveid, killid, row, col);

	GetRowCol(row1, col1, moveid);
	int r = relation(row1, col1, row, col);
	if (r != 1 && r != 10) return false;

	if (col < 3 || col > 5) return false;
	if (isBottomSide(moveid))
	{
		if (row < 7) return false;
	}
	else
	{
		if (row > 2) return false;
	}
	return true;
}

bool Board::canMoveShi(int moveid, int, int row, int col)
{
	GetRowCol(row1, col1, moveid);
	int r = relation(row1, col1, row, col);
	if (r != 11) return false;

	if (col < 3 || col > 5) return false;
	if (isBottomSide(moveid))
	{
		if (row < 7) return false;
	}
	else
	{
		if (row > 2) return false;
	}
	return true;
}

bool Board::canMoveXiang(int moveid, int, int row, int col)
{
	GetRowCol(row1, col1, moveid);
	int r = relation(row1, col1, row, col);
	if (r != 22) return false;

	int rEye = (row + row1) / 2;
	int cEye = (col + col1) / 2;
	if (getStoneId(rEye, cEye) != -1)
		return false;

	if (isBottomSide(moveid))
	{
		if (row < 4) return false;
	}
	else
	{
		if (row > 5) return false;
	}
	return true;
}

bool Board::canMove(int moveid, int killid, int row, int col)
{
	if (sameColor(moveid, killid)) return false;

	switch (_s[moveid]._type)
	{
	case Stone::ROOK:
		return canMoveChe(moveid, killid, row, col);

	case Stone::KNIGHT:
		return canMoveMa(moveid, killid, row, col);

	case Stone::CANNON:
		return canMovePao(moveid, killid, row, col);

	case Stone::PAWN:
		return canMoveBing(moveid, killid, row, col);

	case Stone::KING:
		return canMoveJiang(moveid, killid, row, col);

	case Stone::ADVISOR:
		return canMoveShi(moveid, killid, row, col);

	case Stone::BISHOP:
		return canMoveXiang(moveid, killid, row, col);

	}
	return false;
}

void Board::reliveStone(int id)//复活棋子
{
	if (id == -1) return;
	_s[id]._dead = false;
}

void Board::killStone(int id)//吃棋
{
	if (id == -1) return;
	_s[id]._dead = true;
}

bool Board::isBottomSide(int id)
{
	return _bRedSide == _s[id]._red;
}

void Board::traverseNextStep()
{
	int src = 0;
	for (int i = 0; i < 32; i++)
	{
		switch (_s[i]._type)
		{
		case Stone::KING:
			break;
		case Stone::ADVISOR:
			break;
		case Stone::BISHOP:
			break;
		case Stone::KNIGHT:
			break;
		case Stone::ROOK:
			break;
		case Stone::CANNON:
			break;
		case Stone::PAWN:
			break;
		default:
			break;
		}
	}
}

void Board::slotSave()
{
	QFile fenFile(".\\Fen.txt");
	if (!fenFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		qDebug() << "Could not open file for writing";
		return;
	}
	QTextStream out(&fenFile);
	bool isOk;
	QString name = QInputDialog::getText(this, "保存对局", "请命名对局", QLineEdit::Normal,nullptr,&isOk);
	if (isOk)
	{
		QString fen = _situation.createFen();
		out << name << ':' << fen <<'\n';
		QMessageBox::information(this, "tips", "保存成功");
	}
	fenFile.close();

}

void Board::slotLoad()
{
	LoadDlg * loadDlg = new LoadDlg(this);
	loadDlg->loadFile(".\\Fen.txt");
	if (loadDlg->exec() == QDialog::Accepted)
	{
		QString fen = loadDlg->_fenList->at(loadDlg->_selected);
		load(fen);
		_situation.loadFromFen(fen);
	}
	

}



void Board::moveStone(int moveid, int row, int col)
{
	_s[moveid]._row = row;
	_s[moveid]._col = col;

}

void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps)
{
	GetRowCol(row1, col1, moveid);
	Step* step = new Step;
	step->_colFrom = col1;
	step->_colTo = col;
	step->_rowFrom = row1;
	step->_rowTo = row;
	step->_moveid = moveid;
	step->_killid = killid;

	steps.append(step);

}

void Board::actualMoveStone(int moveid, int killid, int row, int col)
{
	GetRowCol(row1, col1, moveid);//获得要移动的棋子行列
	saveStep(moveid, killid, row, col, _steps);//保存步法
	QString moveStr = createRecord(_steps.last());//获得步法文字描述记录
	emit(sigMove(moveStr));
	_situation.move(row1, col1, row, col);//更新局面

	killStone(killid);
	moveStone(moveid, row, col);

	_bRedTurn = !_bRedTurn;

}

void Board::click(int id, int row, int col)
{
	if (this->_selectid == -1)
	{
		trySelectStone(id);
	}
	else
	{
		tryMoveStone(id, row, col);
	}
}

void Board::click(QPoint pt)
{
	int row, col;
	bool bClicked = getClickRowCol(pt, row, col);
	if (!bClicked) return;

	int id = getStoneId(row, col);
	click(id, row, col);
}

int Board::getStoneId(int row, int col)
{
	for (int i = 0; i < 32; ++i)
	{
		if (_s[i]._row == row && _s[i]._col == col && !isDead(i))
			return i;
	}
	return -1;
}

void Board::back(Step *step)//真正的悔棋操作函数
{
	reliveStone(step->_killid);
	moveStone(step->_moveid, step->_rowFrom, step->_colFrom);

}

void Board::backOne()//提取步法历史表最后一步
{
	if (this->_steps.size() == 0) return;

	Step* step = this->_steps.last();
	_steps.removeLast();
	back(step);
	emit sigBack();
	update();
	delete step;
}

void Board::back()//悔棋函数
{
	backOne();

}
void Board::slotNew()
{
	init(_bRedSide);
}


void Board::slotBack()
{
	back();
}
