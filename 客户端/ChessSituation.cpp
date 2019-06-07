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
	//当目标位置不为0时，即为吃子，拿走目标位置的棋子
	if(pc!=0)
		delPiece(sqDst);
	pc = _board[sqSrc];
	//将棋子从原来位子的拿走，放在目标位置上
	delPiece(sqSrc);
	addPiece(sqDst,pc);
	//更新将的位置
	if (pc==16)
		_sqBKing = sqDst;
	if (pc == 8)
		_sqRKing = sqDst;
	//转变下棋方
	_isRedTurn = 1 - _isRedTurn;
	//判断对方是否被将，杀，捉
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
	//获得历史表最后一步
	MoveStruct *step = _moveHistory.last();
	//获得步法的起点和终点
	int src = step->mv % 256;
	int dst = step->mv / 256;
	//回退棋子
	int pc = _board[dst];
	delPiece(dst);
	addPiece(src, pc);
	//如果吃了子，则要复活棋子
	if (step->killid != 0)
	{
		pc = step->killid;
		addPiece(dst, pc);
	}
	//历史表中删除最后一步
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
	/*红方棋子在棋盘上的值范围是8-14，黑方的范围是16-22，&8不等于0的为红棋，等于0的为黑棋*/
	bool srcColor = _board[src] & 8 != 0;
	bool dstColor = _board[dst] & 8 != 0;
	if (srcColor == dstColor)//颜色相同
		return true;
	return false;
}

bool ChessSituation::awayHalf(int sq)
{
	//当棋子在棋盘上方0-127格时，&128等于0，在下方128-255格时，&128等于128
	//当turn为0时，进7位还是0，当turn为1时，进7位等于128
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
	OPP_TAG = 16 - (_isRedTurn << 3);//获得对方的标记

	// 1. 判断是否被对方的兵(卒)将军
	if (_isRedTurn == 0 && _board[sq - 16] == OPP_TAG + ChessSituation::PAWN)//当下方棋子走时,判断将上方一格棋子是否为对方的兵
	{		
		return true;
	}
	if (_isRedTurn == 1 && _board[sq + 16] == OPP_TAG + ChessSituation::PAWN)//当上方棋子走时，判断将下方一格棋子是否为对方的兵
	{
		return true;
	}
	for (nDelta = -1; nDelta <= 1; nDelta += 2)
	{
		if (_board[sq + nDelta] == OPP_TAG + ChessSituation::PAWN)//判断左右两边的格子是否为对面的兵
		{
			return true;
		}
	}

	// 2. 判断是否被对方的马将军(以仕(士)的步长当作马腿)
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

	// 3. 判断是否被对方的车或炮将军(包括将帅对脸)
	for (i = 0; i < 4; i++)
	{
		nDelta = KingDelta[i];
		sqDst = sq + nDelta;

		while (ccInBoard[sqDst] == 1)
		{
			dstValue = _board[sqDst];
			if (dstValue != 0)//当遇到棋子时
			{
				if (dstValue == OPP_TAG + ChessSituation::KING || dstValue == OPP_TAG + ChessSituation::ROOK)
				{
					return true;
				}
				break;
			}
			sqDst += nDelta;
		}
		sqDst += nDelta;//当有一个炮台的时候，搜索炮
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
			//当找到红方帅时，判断棋盘方向
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
			
			addPiece(getIndexFromRowCol(row, col), stoneType + cRed);//在在棋盘上红方棋子的值为 类型+8
			
			col++;
		}
		if (situation[i].isLower())
		{
			int stoneType = getStoneType(situation[i].toUpper().unicode());
			if (stoneType == 0)
			{
				_sqBKing = getIndexFromRowCol(row, col);
			}
			addPiece(getIndexFromRowCol(row, col), stoneType + cBlack);//在在棋盘上黑方方棋子的值为 类型+16
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
			if (_board[index] == 0)//当前格子没有棋子，间隔+1
			{
				space++;
			}
			else
			{
				if (space > 0)//当间隔不为0时
				{
					QChar num(space + 48);
					fen += num;
					space = 0;
				}
				//因为黑方棋子是16-22，红方棋子是8-15，所以-16大于0的是黑方棋子，小于0的是红方棋子
				int s = _board[index] - 16;
				if (s >= 0)
					fen += cStoneType[s].toLower();
				else
					fen += cStoneType[s + 8];
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
	fen += _isRedTurn ? 'b' : 'w';
	fen += " - - 0 1";
	return fen;
}




