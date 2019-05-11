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
	
	// FEN�������ӱ�ʶ��ע���������ֻ��ʶ���д��ĸ�������Сд��ĸʱ�����ȱ���ת��Ϊ��д
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
			_board[getIndexFromRowCol(row, col)] = stoneType + cRed;//���������Ϻ췽���ӵ�ֵΪ ����+8
			col++;
		}
		if (situation[i].isUpper())
		{
			int stoneType = getStoneType(situation[i].unicode());
			_board[getIndexFromRowCol(row, col)] = stoneType + cBlack;//�����Ϻڷ����ӵ�ֵΪ ����+16
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
			if (_board[index] == 0)//��ǰ����û�����ӣ����+1
			{
				space++;
			}
			else
			{
				if (space > 0)//�������Ϊ0ʱ
				{
					QChar num(space + 48);
					fen += QChar(space + 48);
					space = 0;
				}
				//��Ϊ�ڷ�������16-22���췽������8-15������-16����0���Ǻڷ����ӣ�С��0���Ǻ췽����
				s = _board[index] - 16;
				if (s >= 0)
					fen += cStoneType[s];
				else
					fen += cStoneType[s + 8].toLower();
			}
			
		}
		if (space > 0)//�������Ϊ0ʱ
		{
			QChar num(space + 48);
			fen += QChar(space + 48);
			space = 0;
		}
		fen += '/';
		col = 0;
		row++;
	}
	fen.left(fen.size() - 1);//ȥ����������һ��/
	fen += ' ';
	fen += _bRedTurn ? 'r' : 'b';
	fen += " - - 0 1";
	return fen;
}


//
//int Board::canChaseJiang(int id)
//{
//	int src = getIndexFromRowCol(_s[id]._row, _s[id]._col);//��ý������ڸ��ӵı��
//	int dst = 0;
//	for (int i = 0; i < 4; i++)
//	{
//		dst = src + JiangDelta[i];//�������
//		if (ccInFort[dst] == 1)//�ж��Ƿ��ھŹ�����
//			if (_situation[dst] != 0)//�ж�������
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
//	int src = getIndexFromRowCol(_s[id]._row, _s[id]._col);//���ʿ���ڸ��ӵı��
//	int dst = 0;
//	for (int i = 0; i < 4; i++)
//	{
//		dst = src + ShiDelta[i];//�������
//		if (ccInFort[dst] == 1)//�ж��Ƿ��ھŹ�����
//			if (_situation[dst] != 0)//�ж�������
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
//	int src = getIndexFromRowCol(_s[id]._row, _s[id]._col);//��������ڸ��ӵı��
//	int dst = 0;
//	int eye = 0;//����
//	for (int i = 0; i < 4; i++)
//	{
//		dst = src + XiangDelta[i];//�������
//		eye = src + ShiDelta[i];//��ʿ�������������λ��
//		if (ccInBoard[dst] == 1)//�ж��Ƿ���������
//			if (_situation[eye] == 0)//�ж���û��������
//				if (_situation[dst] != 0)//�ж�������
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
//	int src = getIndexFromRowCol(_s[id]._row, _s[id]._col);//��������ڸ��ӵı��
//	int dst = 0;
//	int leg = 0;//����
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 2; j++)
//		{
//			dst = src + MaDelta[i][j];//�������
//			leg = src + JiangDelta[i];//��˧�������������λ��
//			if (ccInBoard[dst] == 1)//�ж��Ƿ���������
//				if (_situation[leg] == 0)//�ж���û�а�����
//					if (_situation[dst] != 0)//�ж�������
//					{
//						return (_situation[dst] & 8 != 0) ? 0 : _situation[dst];
//					}
//		}
//
//	}
//	return 0;
//}
