#include "ChessSituation.h"

ChessSituation::ChessSituation(bool direction)
	:_board(new char[256]),_isRedTurn(0),_direction(direction)
{	
	_sqBKing = getIndexFromRowCol(0, 4);
	_sqRKing = getIndexFromRowCol(9, 4);
	_redValue = 0;
	_blackValue = 0;
	memset(_board, 0, 256);

	for (int i = 0,pc=0; i < 256; i++) {
		pc = InitialBoard[i];
		if (pc != 0) {
			addPiece(i, pc);
		}
	}
	if (!direction)
	{
		flipSquare();
	}
}

int ChessSituation::getIndexFromRowCol(int row, int col)
{
	return row*16+col+0x33;
}



void ChessSituation::move(int sqSrc, int sqDst)
{
	int pc = _board[sqDst];
	//��Ŀ��λ�ò�Ϊ0ʱ����Ϊ���ӣ�����Ŀ��λ�õ�����
	if(pc!=0)
		delPiece(sqDst);
	pc = _board[sqSrc];
	//�����Ӵ�ԭ��λ�ӵ����ߣ�����Ŀ��λ����
	delPiece(sqSrc);
	addPiece(sqDst,pc);
	//���½���λ��
	if (pc==16)
		_sqBKing = sqDst;
	if (pc == 8)
		_sqRKing = sqDst;
	//ת�����巽
	_isRedTurn = 1 - _isRedTurn;
	//�ж϶Է��Ƿ񱻽���ɱ��׽
	bool checked = check();
	bool mating = mate();
	bool chased = chase();
	int mv = sqSrc + sqDst * 256;
	MoveStruct *step=new MoveStruct(mv, _board[sqDst],checked);
	_moveHistory.append(step);
}
void ChessSituation::move(int srcRow, int srcCol, int dstRow, int dstCol)
{
	int src = getIndexFromRowCol(srcRow, srcCol);
	int dst = getIndexFromRowCol(dstRow, dstCol);
	move(src,dst );
}
void ChessSituation::back()
{
	if (_moveHistory.size() == 0) 
		return;
	//�����ʷ�����һ��
	MoveStruct *step = _moveHistory.last();
	//��ò����������յ�
	int src = step->mv % 256;
	int dst = step->mv / 256;
	//��������
	int pc = _board[dst];
	delPiece(dst);
	addPiece(src, pc);
	//��������ӣ���Ҫ��������
	if (step->killid != 0)
	{
		pc = step->killid;
		addPiece(dst, pc);
	}
	//��ʷ����ɾ�����һ��
	delete step;
	_moveHistory.removeLast();
}
int ChessSituation::getSqX(int sq)
{
	return sq >> 4;
}

int ChessSituation::getSqY(int sq)
{
	return sq & 15;
}

bool ChessSituation::sameColor(int src, int dst)
{
	/*�췽�����������ϵ�ֵ��Χ��8-14���ڷ��ķ�Χ��16-22��&8������0��Ϊ���壬����0��Ϊ����*/
	bool srcColor = _board[src] & 8 != 0;
	bool dstColor = _board[dst] & 8 != 0;
	if (srcColor == dstColor)//��ɫ��ͬ
		return true;
	return false;
}

bool ChessSituation::awayHalf(int sq)
{
	//�������������Ϸ�0-127��ʱ��&128����0�����·�128-255��ʱ��&128����128
	//��turnΪ0ʱ����7λ����0����turnΪ1ʱ����7λ����128
	if (_direction)
	{
		return ((sq & 0x80) == (_isRedTurn << 7));
	}
	else
	{
		return ((sq & 0x80) != (_isRedTurn << 7));
	}

}

int ChessSituation::flipSquare(int sq)
{
	return 254-sq;
}

void ChessSituation::flipSquare()
{
	for (int i=0;i<256;i++)
	{
		int j = flipSquare(i);
		_board[i] ^= _board[j];
		_board[j] ^= _board[i];
		_board[i] ^= _board[j];
	}
}

bool ChessSituation::check()
{
	int i, j, sq, sqDst;
	int OPP_TAG, dstValue, nDelta;
	if (_isRedTurn)
		sq = _sqBKing;
	else
		sq = _sqRKing;
	OPP_TAG = 16 - (_isRedTurn << 3);//��öԷ��ı��

	// 1. �ж��Ƿ񱻶Է��ı�(��)����
	if (_isRedTurn == 0 && _board[sq - 16] == OPP_TAG + ChessSituation::PAWN)//���·�������ʱ,�жϽ��Ϸ�һ�������Ƿ�Ϊ�Է��ı�
	{		
		return true;
	}
	if (_isRedTurn == 1 && _board[sq + 16] == OPP_TAG + ChessSituation::PAWN)//���Ϸ�������ʱ���жϽ��·�һ�������Ƿ�Ϊ�Է��ı�
	{
		return true;
	}
	for (nDelta = -1; nDelta <= 1; nDelta += 2)
	{
		if (_board[sq + nDelta] == OPP_TAG + ChessSituation::PAWN)//�ж��������ߵĸ����Ƿ�Ϊ����ı�
		{
			return true;
		}
	}

	// 2. �ж��Ƿ񱻶Է�������(����(ʿ)�Ĳ�����������)
	for (i = 0; i < 4; i++)
	{
		if (_board[sq + AdvisorDelta[i]] != 0)
		{
			continue;
		}
		for (j = 0; j < 2; j++)
		{
			dstValue = _board[sq + ccKnightCheckDelta[i][j]];
			if (dstValue == OPP_TAG + ChessSituation::KNIGHT)
			{
				return true;
			}
		}
	}

	// 3. �ж��Ƿ񱻶Է��ĳ����ڽ���(������˧����)
	for (i = 0; i < 4; i++)
	{
		nDelta = KingDelta[i];
		sqDst = sq + nDelta;

		while (ccInBoard[sqDst] == 1)
		{
			dstValue = _board[sqDst];
			if (dstValue != 0)//����������ʱ
			{
				if (dstValue == OPP_TAG + ChessSituation::KING || dstValue == OPP_TAG + ChessSituation::ROOK)
				{
					return true;
				}
				break;
			}
			sqDst += nDelta;
		}
		sqDst += nDelta;//����һ����̨��ʱ��������
		while (ccInBoard[sqDst] == 1)
		{
			int pcDst = _board[sqDst];
			if (pcDst != 0) {
				if (pcDst == OPP_TAG + ChessSituation::CANNON)
				{
					return true;
				}
				break;
			}
			sqDst += nDelta;
		}
	}
	return false;
	

}

bool ChessSituation::chase()
{
	return false;
}

bool ChessSituation::mate()
{
	return false;
}

void ChessSituation::addPiece(int sq, int pc)
{
	_board[sq] = pc;
	if (pc < 16) 
	{
		_redValue += cucvlPiecePos[pc - 8][sq];
	}
	else 
	{
		_blackValue += cucvlPiecePos[pc - 16][flipSquare(sq)];		
	}
}

void ChessSituation::delPiece(int sq)
{
	int pc = _board[sq];
	_board[sq] = 0;
	if (pc < 16)
	{
		_redValue -= cucvlPiecePos[pc - 8][sq];
	}
	else
	{
		_blackValue -= cucvlPiecePos[pc - 16][flipSquare(sq)];
	}
}

int ChessSituation::evaluate(void)const
{
	return (_isRedTurn == 0 ? _redValue - _blackValue : _blackValue - _redValue);
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
	_isRedTurn = turn.contains('b')?1:0;
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
		if (situation[i].isUpper())
		{
			int stoneType=getStoneType(situation[i].unicode());
			//���ҵ��췽˧ʱ���ж����̷���
			if (stoneType == 0)
			{
				_sqRKing = getIndexFromRowCol(row, col);
				if (row<3)
				{
					_direction = false;
				} 
				else
				{
					_direction = true;
				}
			}
			
			addPiece(getIndexFromRowCol(row, col), stoneType + cRed);//���������Ϻ췽���ӵ�ֵΪ ����+8
			
			col++;
		}
		if (situation[i].isLower())
		{
			int stoneType = getStoneType(situation[i].toUpper().unicode());
			if (stoneType == 0)
			{
				_sqBKing = getIndexFromRowCol(row, col);
			}
			addPiece(getIndexFromRowCol(row, col), stoneType + cBlack);//���������Ϻڷ������ӵ�ֵΪ ����+16
			col++;
		}
		if (situation[i].isDigit())
		{
			col += situation[i].unicode() - 48;
		}
	}
	_moveHistory.clear();
		
}

QString ChessSituation::createFen()
{
	QString fen;
	int row = 0, col = 0,space=0;
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
					fen += num;
					space = 0;
				}
				//��Ϊ�ڷ�������16-22���췽������8-15������-16����0���Ǻڷ����ӣ�С��0���Ǻ췽����
				int s = _board[index] - 16;
				if (s >= 0)
					fen += cStoneType[s].toLower();
				else
					fen += cStoneType[s + 8];
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
	fen += _isRedTurn ? 'b' : 'w';
	fen += " - - 0 1";
	return fen;
}




