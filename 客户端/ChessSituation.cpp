#include "ChessSituation.h"

ChessSituation::ChessSituation()
	:_board(new char[256]),_bRedTurn(true)
{
	memcpy(_board, InitialBoard,256);
	_moveHistory = new QVector<MoveStep>;
}

int ChessSituation::getIndexFromRowCol(int row, int col)
{
	return row*16+col+0x33;
}



void ChessSituation::move(int sqSrc, int sqDst)
{
	_board[sqDst] = _board[sqSrc];
	_board[sqSrc] = 0;
	_bRedTurn = !_bRedTurn; 
	int mv = sqSrc + sqDst * 256;
	MoveStep step(mv, _board[sqDst]);
	_moveHistory->append(step);
}
void ChessSituation::move(int srcRow, int srcCol, int dstRow, int dstCol)
{
	move(getIndexFromRowCol(srcRow, srcCol), getIndexFromRowCol(dstRow, dstCol));
}
void ChessSituation::back()
{
	if (this->_moveHistory->size() == 0) return;
	MoveStep step = this->_moveHistory->last();
	int src = step.mv % 256;
	int dst = step.mv / 256;
	_board[src] = _board[dst];
	_board[dst] = step.killid;
	_moveHistory->removeLast();
}
int ChessSituation::getSqX(int sq)
{
	return sq >> 4;
}

int ChessSituation::getSqY(int sq)
{
	return sq & 15;
}

int ChessSituation::getStoneType(char stone)
{
	
	// FEN串中棋子标识，注意这个函数只能识别大写字母，因此用小写字母时，首先必须转换为大写
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

void ChessSituation::loadFromFen(QString Fen)
{
	QString situation = Fen.section(' ', 0, 0);
	QString turn = Fen.section(' ', 1, 1);
	_bRedTurn = turn.contains('b')?0:1;
	int row = 0, col = 0;
	for (int i = 0; i < 256; i++)
		_board[i] = 0;
	for (int i = 0; i < situation.size(); i++)
	{
		if (situation[i] == '/')
		{
			row++;
			col = 0;
		}
		if (situation[i].isLower())
		{
			int stoneType=getStoneType(situation[i].toUpper().unicode());
			_board[getIndexFromRowCol(row, col)] = stoneType + cRed;//在在棋盘上红方棋子的值为 类型+8
			col++;
		}
		if (situation[i].isUpper())
		{
			int stoneType = getStoneType(situation[i].unicode());
			_board[getIndexFromRowCol(row, col)] = stoneType + cBlack;//棋盘上黑方棋子的值为 类型+16
			col++;
		}
		if (situation[i].isDigit())
		{
			col += situation[i].unicode() - 48;
		}
	}
	_moveHistory->clear();
		
}

QString ChessSituation::createFen()
{
	QString fen;
	int row = 0, col = 0,space=0,s;
	while (row < 10)
	{
		int index = getIndexFromRowCol(row, col);
		for (col = 0; col < 9; col++,index++)
		{		
			if (_board[index] == 0)//当前格子没有棋子，间隔+1
			{
				space++;
			}
			else
			{
				if (space > 0)//当间隔不为0时
				{
					QChar num(space + 48);
					fen += QChar(space + 48);
					space = 0;
				}
				//因为黑方棋子是16-22，红方棋子是8-15，所以-16大于0的是黑方棋子，小于0的是红方棋子
				s = _board[index] - 16;
				if (s >= 0)
					fen += cStoneType[s];
				else
					fen += cStoneType[s + 8].toLower();
			}
			
		}
		if (space > 0)//当间隔不为0时
		{
			QChar num(space + 48);
			fen += QChar(space + 48);
			space = 0;
		}
		fen += '/';
		col = 0;
		row++;
	}
	fen.left(fen.size() - 1);//去掉在最后多余一个/
	fen += ' ';
	fen += _bRedTurn ? 'r' : 'b';
	fen += " - - 0 1";
	return fen;
}


//
//int Board::canChaseJiang(int id)
//{
//	int src = getIndexFromRowCol(_s[id]._row, _s[id]._col);//获得将军所在格子的编号
//	int dst = 0;
//	for (int i = 0; i < 4; i++)
//	{
//		dst = src + JiangDelta[i];//获得增量
//		if (ccInFort[dst] == 1)//判断是否在九宫格内
//			if (_situation[dst] != 0)//判断有无子
//			{
//				return (_situation[dst] & 8 != 0) ? 0 : _situation[dst];
//			}
//
//	}
//	return 0;
//}
//
//int Board::canChaseShi(int id)
//{
//	int src = getIndexFromRowCol(_s[id]._row, _s[id]._col);//获得士所在格子的编号
//	int dst = 0;
//	for (int i = 0; i < 4; i++)
//	{
//		dst = src + ShiDelta[i];//获得增量
//		if (ccInFort[dst] == 1)//判断是否在九宫格内
//			if (_situation[dst] != 0)//判断有无子
//			{
//				return (_situation[dst] & 8 != 0) ? 0 : _situation[dst];
//			}
//
//	}
//	return 0;
//}
//
//int Board::canChaseXiang(int id)
//{
//	int src = getIndexFromRowCol(_s[id]._row, _s[id]._col);//获得象所在格子的编号
//	int dst = 0;
//	int eye = 0;//象眼
//	for (int i = 0; i < 4; i++)
//	{
//		dst = src + XiangDelta[i];//获得增量
//		eye = src + ShiDelta[i];//以士的增量获得象眼位置
//		if (ccInBoard[dst] == 1)//判断是否在棋盘内
//			if (_situation[eye] == 0)//判断有没有塞象眼
//				if (_situation[dst] != 0)//判断有无子
//				{
//					return (_situation[dst] & 8 != 0) ? 0 : _situation[dst];
//				}
//
//	}
//	return 0;
//}
//
//int Board::canChaseMa(int id)
//{
//	int src = getIndexFromRowCol(_s[id]._row, _s[id]._col);//获得象所在格子的编号
//	int dst = 0;
//	int leg = 0;//马腿
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 2; j++)
//		{
//			dst = src + MaDelta[i][j];//获得增量
//			leg = src + JiangDelta[i];//以帅的增量获得马腿位置
//			if (ccInBoard[dst] == 1)//判断是否在棋盘内
//				if (_situation[leg] == 0)//判断有没有绊马腿
//					if (_situation[dst] != 0)//判断有无子
//					{
//						return (_situation[dst] & 8 != 0) ? 0 : _situation[dst];
//					}
//		}
//
//	}
//	return 0;
//}
